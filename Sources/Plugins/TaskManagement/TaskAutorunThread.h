#ifndef TASKAUTORUNTHREAD_H
#define TASKAUTORUNTHREAD_H

#include <QThread>

class Task;
class TaskManager;

// This thread keeps checking whether a task should be run every "checkInterval" second(s).
// If a task should be run, emits signal "shouldStartTask(Task* task)".
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
