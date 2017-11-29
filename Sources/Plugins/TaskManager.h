#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QThread>
#include <QVector>
#include <QDateTime>
#include <QMutex>
#include <QDebug>

#include "Task.h"

class TaskManager : public QThread {
public:
    TaskManager();
    ~TaskManager() {
        for (Task* t : tasks) {
            t->terminate();
            delete t;
        }
        terminate();
        wait();
    }

    void runTaskAt(int index) {
        tasks[index]->start();
    }

    void stopTaskAt(int index) {
        tasks[index]->terminate();
    }

    void stop() {
        manuallyStopped = true;
    }

    Task* create(const QString& name,
                 const QString& description,
                 const QDateTime& startTime,
                 Trigger::Internal internal,
                 const QString& operation,
                 const QStringList& arguments) {
        return new Task(name, description, startTime, internal, operation, arguments);
    }

    // Duplicate basic infomations only,
    // without its running state.
    Task* duplicate(Task* rhs) {
        return new Task(*rhs);
    }

    void append(Task* task) {
        tasks.append(task);
    }

    void insert(Task* task, int index) {
        tasks.insert(index, task);
    }

    void remove(int index) {
        tasks[index]->terminate();
        tasks.remove(index);
    }

    Task* get(int index) const {
        return tasks[index];
    }

    int size() const {
        return tasks.size();
    }

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
            sleep(1);
        }
    }

    void runAll() {
        QMutexLocker locker(&mutex);
        for (Task* t : tasks) {
            t->start();
        }
    }

private:
    bool manuallyStopped = false;
    QVector<Task*> tasks;
    mutable QMutex mutex;

};

#endif // TASKMANAGER_H
