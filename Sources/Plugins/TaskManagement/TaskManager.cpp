#include "TaskManager.h"

TaskManager::TaskManager(qint64 checkInterval)
    : checkInterval(checkInterval),
      autorunThread(this)
{
    manuallyStopped = false;

    connect(&autorunThread, &TaskAutorunThread::shouldStartTask,
            this,           &TaskManager::startTask);
    autorunThread.start();
}

qint64 TaskManager::getCheckInterval() const {
    return checkInterval;
}

void TaskManager::setCheckInterval(qint64 value) {
    checkInterval = value;
}
