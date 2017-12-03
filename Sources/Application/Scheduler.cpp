#include "Scheduler.h"

Scheduler::Scheduler(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(kAppName);
    setWindowIcon(QIcon(":/images/task-management.png"));
    resize(900, 600);

    setupToolBar();
    setupTaskTableWidget();
    setupSystemTray();

    connect(&taskManager, &TaskManager::taskStateChanged,
            this,         &Scheduler::updateRow);
}

Scheduler::~Scheduler() {
    taskManager.stop();

    systemTray->hide();
    delete systemTray;
    delete wizard;
    delete settingDialog;
}
