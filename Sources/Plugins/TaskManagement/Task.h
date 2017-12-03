#ifndef TASK_H
#define TASK_H

#include <QThread>
#include <QString>
#include <QVector>
#include <QProcess>
#include <QDateTime>
#include <QMap>
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
    }

    void kill() {
        for (Operation* op : operations) {
            op->killActiveProcess();
        }
    }

    bool shouldRun() const {
        QDateTime nextStartTime = getNextStartTime();
        return nextStartTime.isValid() && nextStartTime <= QDateTime::currentDateTime();
    }

    QMap<QProcess::ProcessState, int> getState() const {
        QMap<QProcess::ProcessState, int> ret;
        ret[QProcess::Running] = ret[QProcess::Starting] = ret[QProcess::NotRunning] = 0;
        for (Operation* op : operations) {
            ++ret[op->process.state()];
        }
        return ret;
    }

    // Returns invalid QDateTime object if all triggers aren't periodic trigger.
    QDateTime getNextStartTime() const {
        QDateTime time;
        for (Trigger* t : triggers) {
            if (t->isPeriodic()) {
                time = time.isValid() ? std::min(time, t->nextStartTime) : t->nextStartTime;
            }
        }
        return time;
    }

    const QString& getName() const;
    void setName(const QString& value);

    const QString& getDescription() const;
    void setDescription(const QString& value);

    const QDateTime& getLastStartTime() const;
    const QString& getLastRunResult() const;
    const QVector<Operation*>& getOperations() const;
    const QVector<Trigger*>& getTriggers() const;

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

    Task(const Task& rhs) = delete;

    ~Task() {
        for (Trigger* t : triggers) {
            delete t;
        }
        for (Operation* o : operations) {
            delete o;
        }
    }

    void startOperations() {
        for (Operation* op : operations) {
            if (op->process.state() == QProcess::NotRunning) {
                lastStartTime = QDateTime::currentDateTime();
                QProcess& p = op->process;
                p.setProgram(op->program);
                p.setArguments(op->arguments);
                p.start();
            } else {
                qDebug() << "Cannot start because this operation has been actived already!";
            }
        }
    }

    void updateTriggers() {
        bool hasUpdated = false;
        for (Trigger* t : triggers) {
            if (t->triggered()) {
                hasUpdated = true;
                t->updateNextStartTime();
            }
        }
        if (hasUpdated) {
            emit stateChanged();
        }
    }

private:
    QString name;
    QString description;
    QVector<Operation*> operations;
    QVector<Trigger*> triggers;
    QDateTime lastStartTime;
    QString lastRunResult;

};

#endif // TASK_H
