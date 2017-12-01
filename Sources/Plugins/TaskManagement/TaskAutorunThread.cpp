#include "TaskAutorunThread.h"
#include "TaskManager.h"

void TaskAutorunThread::run() {
    while (!taskManager->manuallyStopped) {
        QVector<Task*>& tasks = taskManager->tasks;
        int checkInterval = taskManager->checkInterval;

        for (Task* t : tasks) {
            if (t->shouldRun()) {
                t->start();
            }
        }
        sleep(checkInterval);
    }
}
