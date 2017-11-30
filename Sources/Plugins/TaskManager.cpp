#include "TaskManager.h"

TaskManager::TaskManager(int checkInterval)
    : checkInterval(checkInterval)
{
    start();
}

int TaskManager::getCheckInterval() const {
    return checkInterval;
}

void TaskManager::setCheckInterval(int value) {
    checkInterval = value;
}
