#include "TaskManager.h"

TaskManager::TaskManager(int checkInterval)
    : checkInterval(checkInterval),
      autorunThread(this)
{
    manuallyStopped = false;

    connect(&autorunThread, &TaskAutorunThread::shouldStartTask,
            this,           &TaskManager::startTask);
    autorunThread.start();
}

int TaskManager::getCheckInterval() const {
    return checkInterval;
}

void TaskManager::setCheckInterval(int value) {
    checkInterval = value;
}
