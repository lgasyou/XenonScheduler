#include "Scheduler.h"

Scheduler::Scheduler(QWidget *parent) :
    QMainWindow(parent),
    wizard(&taskManager, this),
    systemTray(this)
{
    setWindowTitle(kAppName);
    setWindowIcon(QIcon(":/images/task-management.png"));
    resize(900, 600);

    setupToolBar();
    setupTaskTable();
    setupSystemTray();

    connect(&taskManager, &TaskManager::taskStateChanged,
            this,         &Scheduler::updateRow);
}

Scheduler::~Scheduler() {
    taskManager.stop();
}
