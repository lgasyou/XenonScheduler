#ifndef TASKAUTORUNTHREAD_H
#define TASKAUTORUNTHREAD_H

#include <QThread>

class TaskManager;

class TaskAutorunThread : public QThread {
    Q_OBJECT

public:
    TaskAutorunThread(TaskManager* taskManager)
        : taskManager(taskManager)
    {}

private:
    void run() override;

private:
    TaskManager* taskManager;
};

#endif // TASKAUTORUNTHREAD_H
