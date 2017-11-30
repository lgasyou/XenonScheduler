#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QThread>
#include <QVector>
#include <QDateTime>
#include <QMutex>
#include <QDebug>

#include "Task.h"

class TaskManager : public QThread {
    Q_OBJECT

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

    TaskManager(int checkInterval = 1);
    ~TaskManager() {
        mutex.lock();
        for (Task* t : tasks) {
            t->kill();
            delete t;
        }
        mutex.unlock();
        terminate();
        wait();
    }

    void runTaskAt(int index) {
        tasks[index]->start();
    }

    void stopTaskAt(int index) {
        tasks[index]->kill();
    }

    void stop() {
        manuallyStopped = true;
    }

    // Duplicate basic infomations only,
    // without its runtime state.
    Task* duplicate(Task* rhs) {
        return new Task(*rhs);
    }

    void append(Task* task) {
        QMutexLocker locker(&mutex);
        tasks.append(task);
        connectTaskSignal(task);
    }

    void insert(Task* task, int index) {
        QMutexLocker locker(&mutex);
        tasks.insert(index, task);
        connectTaskSignal(task);
    }

    void remove(int index) {
        tasks[index]->kill();
        delete tasks[index];
        QMutexLocker locker(&mutex);
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

    int getCheckInterval() const;
    void setCheckInterval(int value);

signals:
    void taskStateChanged(int index);

private:
    void run() override {
        manuallyStopped = false;
        runPending();
    }

    void runPending() {
        while (!manuallyStopped) {
            mutex.lock();
            for (Task* t : tasks) {
                if (t->shouldRun()) {
                    t->start();
                }
            }
            mutex.unlock();
            sleep(checkInterval);
        }
    }

    void connectTaskSignal(Task* task) {
        connect(task, &Task::stateChanged, [=]() {
            int index = indexOf(task);
            emit taskStateChanged(index);
        });
    }

private:
    int checkInterval;
    QVector<Task*> tasks;
    bool manuallyStopped = false;
    mutable QMutex mutex;

};

#endif // TASKMANAGER_H