#ifndef TASK_H
#define TASK_H

#include <QThread>
#include <QString>
#include <QVector>
#include <QProcess>
#include <QDateTime>
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
        startOperations();
        updateTriggers();
        emit stateChanged();
    }

    void kill() {
        for (Operation& op : operations) {
            op.killActiveProcess();
        }
    }

    bool shouldRun() const {
        QDateTime nextStartTime = getNextStartTime();
        return nextStartTime.isValid() && nextStartTime <= QDateTime::currentDateTime();
    }

    // Priority: Running > Starting > Stopped.
    QString getState() const {
        int runningCnt = 0, startingCnt = 0, notRunningCnt = 0;
        for (const Operation& op : operations) {
            QProcess* p = op.process;
            if (p->state() == QProcess::Running) {
                ++runningCnt;
            } else if (p->state() == QProcess::Starting) {
                ++startingCnt;
            } else if (p->state() == QProcess::NotRunning) {
                ++notRunningCnt;
            }
        }
        return QString("%1 Running, %2 Starting, %3 Not Running").arg(runningCnt).arg(startingCnt).arg(notRunningCnt);
    }

    // Returns invalid QDateTime object if all triggers aren't periodic trigger.
    QDateTime getNextStartTime() const {
        QDateTime time;
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
        for (Operation& op : operations) {
            if (op.process->state() != QProcess::NotRunning) {
                continue;
            }

            QProcess*& p = op.process;
            p->setProgram(op.program);
            p->setArguments(op.arguments);
            p->start();
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

};

#endif // TASK_H
