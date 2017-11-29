#include "Scheduler.h"

Scheduler::Scheduler(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(kAppName);
    setWindowIcon(QIcon(":/images/task-management.png"));
    resize(900, 600);

    setupToolBar();
    setupTableWidget();
    setupSystemTray();
}

Scheduler::~Scheduler() {
    taskManager.stop();

    delete systemTray;
    delete wizard;
    delete settingDialog;
}
