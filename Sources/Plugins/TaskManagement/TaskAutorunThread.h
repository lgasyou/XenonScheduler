#ifndef TASKAUTORUNTHREAD_H
#define TASKAUTORUNTHREAD_H

#include <QThread>

class Task;
class TaskManager;

class TaskAutorunThread : public QThread {
    Q_OBJECT

public:
    TaskAutorunThread(TaskManager* taskManager)
        : taskManager(taskManager)
    {}

signals:
    void shouldStartTask(Task* task);

private:
    void run() override;

private:
    TaskManager* taskManager;
};

#endif // TASKAUTORUNTHREAD_H
