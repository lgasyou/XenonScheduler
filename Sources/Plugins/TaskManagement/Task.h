#ifndef TASK_H
#define TASK_H

#include <QObject>
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

class Task : public QObject {
    Q_OBJECT

    friend class TaskManager;

public:
    void start() {
        {
            QMutexLocker locker(&mutex);
            startOperations();
            updateTriggers();
        }

        emit stateChanged();
    }

    void kill() {
        QMutexLocker locker(&mutex);
        for (auto p : processes) {
            p->kill();
            qDebug() << p->state();
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

signals:
    void stateChanged();

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

    void startOperations() {
        for (const Operation& op : operations) {
            QProcess* p = new QProcess();
            p->setProgram(op.program);
            p->setArguments(op.arguments);
            p->start();
            processes.append(p);
            qDebug() << "Task::start()";

            // FIXME: won't output anything.
            connect(p, &QProcess::readyReadStandardOutput, [=]() {
                qDebug() << p->readAllStandardOutput().data();
            });
            connect(p, &QProcess::stateChanged, [=]() {
                emit stateChanged();
            });
            connect(p, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                    [=](int code, QProcess::ExitStatus s) {
                qDebug() << code << " " << s;
                processes.removeOne(p);
                delete p;
            });
        }
    }

    void updateTriggers() {
        for (Trigger& t : triggers) {
            if (t.triggered()) {
                t.updateNextStartTime();
            }
        }
    }

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
