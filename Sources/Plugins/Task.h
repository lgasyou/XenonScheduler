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
            QObject::connect(p, &QProcess::readyReadStandardOutput, [=]() {
                qDebug() << p->readAllStandardOutput().data();
            });
            processes.append(p);
        }
    }

    void terminate() {
        QMutexLocker locker(&mutex);
        for (auto p : processes) {
            if (p->state() == QProcess::Running || p->state() == QProcess::Starting) {
                p->terminate();
                processes.removeOne(p);
            }
        }
    }

    bool shouldRun() const {
        return getNextStartTime() <= QDateTime::currentDateTime();
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

    QDateTime getNextStartTime() const {
        QDateTime time;
        QMutexLocker locker(&mutex);
        for (const Trigger& t : triggers) {
            time = std::min(time, t.nextStartTime);
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
         Trigger::Internal internal,
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
