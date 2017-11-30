#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QVector>
#include <QProcess>
#include <QDateTime>
#include <QMutex>
#include <QDebug>
#include <algorithm>

#include "Operation.h"
#include "Trigger.h"

class TaskManager;

class Task {
    friend class TaskManager;

public:
    void start() {
        QMutexLocker locker(&mutex);
        for (const Operation& op : operations) {
            QProcess* p = new QProcess();
            p->setProgram(op.program);
            p->setArguments(op.arguments);
            p->start();
            qDebug() << "Task::start()";
			// FIXME: won't output anything.
            QObject::connect(p, &QProcess::readyReadStandardOutput, [=]() {
                qDebug() << p->readAllStandardOutput().data();
            });
            QObject::connect(p, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                             [=](int code, QProcess::ExitStatus s) {
                qDebug() << code << " " << s;
                processes.removeOne(p);
                delete p;
            });
            processes.append(p);
        }
        for (Trigger& t : triggers) {
            if (t.triggered()) {
                t.updateNextStartTime();
                qDebug() << t.nextStartTime;
            }
        }
    }

    void terminate() {
        QMutexLocker locker(&mutex);
        for (auto p : processes) {
            p->kill();
        }
    }

    bool shouldRun() const {
        QDateTime nextStartTime = getNextStartTime();
        return nextStartTime.isValid() && nextStartTime <= QDateTime::currentDateTime();
    }

    // Priority: Running > Starting > Stopped.
    QString getState() const {
        bool hasOneProcessWhichIsStarting = false;
        QMutexLocker locker(&mutex);
        for (auto p : processes) {
            if (p->state() == QProcess::Running) {
                return "Running";
            }
            if (p->state() == QProcess::Starting) {
                hasOneProcessWhichIsStarting = true;
            }
        }
        return hasOneProcessWhichIsStarting ? "Starting" : "Stopped";
    }

    // Returns invalid QDateTime object if all triggers aren't periodic trigger.
    QDateTime getNextStartTime() const {
        QDateTime time;
        QMutexLocker locker(&mutex);
        for (const Trigger& t : triggers) {
            if (t.isPeriodic()) {
                time = time.isValid() ? std::min(time, t.nextStartTime) : t.nextStartTime;
            }
        }
        return time;
    }

    QString getName() const;
    void setName(const QString& value);

    QString getDescription() const;
    void setDescription(const QString& value);

    QDateTime getLastStartTime() const;
    void setLastStartTime(const QDateTime& value);

    QString getLastRunResult() const;
    void setLastRunResult(const QString& value);

private:
    Task(const QString& name,
         const QString& description,
         const QDateTime& startTime,
         Trigger::IntervalType internalType,
         qint64 interval,
         const QString& operation,
         const QStringList& arguments);

    Task(const Task& rhs)
        : name(rhs.name),
          description(rhs.description),
          operations(rhs.operations),
          triggers(rhs.triggers)
    {}

private:
    QString name;
    QString description;
    QVector<Operation> operations;
    QVector<Trigger> triggers;
    QDateTime lastStartTime;
    QString lastRunResult;
    QList<QProcess*> processes;

    mutable QMutex mutex;

};

#endif // TASK_H
