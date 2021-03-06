#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QVector>
#include <QDateTime>
#include <QDebug>

#include "Task.h"
#include "TaskAutorunThread.h"

class TaskManager : public QObject {
    Q_OBJECT

    friend class TaskAutorunThread;
    friend QDataStream& operator<<(QDataStream&, const TaskManager&);
    friend QDataStream& operator>>(QDataStream&, TaskManager&);

public:
    static Task* create(const QString& name,
                 const QString& description,
                 const QDateTime& startTime,
                 Trigger::IntervalType internalType,
                 qint64 interval,
                 const QString& operation,
                 const QStringList& arguments) {
        return new Task(name, description, startTime, internalType, interval, operation, arguments);
    }

    TaskManager(qint64 checkInterval = 1);
    ~TaskManager() {
        autorunThread.terminate();
        autorunThread.wait();
        for (Task* t : tasks) {
            t->kill();
            delete t;
        }
    }

    void startTaskAt(int index) {
        tasks[index]->start();
    }

public slots:
    void startTask(Task* task) {
        task->start();
    }

public:
    void stopTaskAt(int index) {
        tasks[index]->kill();
    }

    void stop() {
        manuallyStopped = true;
    }

    void append(Task* task) {
        tasks.append(task);
        connectTaskSignal(task);
    }

    void insert(Task* task, int index) {
        tasks.insert(index, task);
        connectTaskSignal(task);
    }

    void remove(int index) {
        tasks[index]->kill();
        delete tasks[index];
        tasks.remove(index);
    }

    Task* get(int index) const {
        return tasks[index];
    }

    int indexOf(Task* task) const {
        return tasks.indexOf(task);
    }

    int size() const {
        return tasks.size();
    }

    qint64 getCheckInterval() const;
    void setCheckInterval(qint64 value);

signals:
    void taskStateChanged(int index);

private:
    void connectTaskSignal(Task* task) {
        connect(task, &Task::stateChanged, [=]() {
            int index = indexOf(task);
            qDebug() << task;
            emit taskStateChanged(index);
        });
    }

private:
    qint64 checkInterval;
    QVector<Task*> tasks;
    TaskAutorunThread autorunThread;

    bool manuallyStopped = false;

};

#endif // TASKMANAGER_H
