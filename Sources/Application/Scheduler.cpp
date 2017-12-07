#include "Scheduler.h"

#include "Plugins/Persistence/TaskManagerSerializer.h"

static QString ProcessState2StringHelper(const QMap<QProcess::ProcessState, int>& states);

Scheduler::Scheduler(QWidget *parent) :
    QMainWindow(parent),
    wizard(&taskManager, this),
    systemTray(this)
{
    setWindowTitle(kAppName);
    setWindowIcon(QIcon(":/images/scheduler.png"));
    resize(900, 600);

    setupToolBar();
    setupTaskTable();
    setupSystemTray();

    connect(&taskManager, &TaskManager::taskStateChanged,
            this,         &Scheduler::updateRow);
    TaskManagerSerializer().deserialize(taskManager);
    loadTasksFromTaskManager();
}

Scheduler::~Scheduler() {
    taskManager.stop();
    TaskManagerSerializer().serialize(taskManager);
}

void Scheduler::insertRow(Task *task, int index) {
    QTableWidget* t = taskTable;
    if (index == -1) {
        taskManager.insert(task, t->rowCount());
    }
    insertRowWithoutChangeTaskManager(task, index);
}

void Scheduler::updateRow(int index) {
    qDebug() << "Scheduler::updateRow(int)";
    Task* task = taskManager.get(index);
    taskTable->setItem(index, 0, new QTableWidgetItem(task->getName()));
    taskTable->setItem(index, 1, new QTableWidgetItem(::ProcessState2StringHelper(task->getState())));
    taskTable->setItem(index, 2, new QTableWidgetItem(task->getNextStartTime().toString()));
    taskTable->setItem(index, 3, new QTableWidgetItem(task->getLastStartTime().toString()));
    taskTable->setItem(index, 4, new QTableWidgetItem(task->getLastRunResult()));
}

void Scheduler::insertRowWithoutChangeTaskManager(Task *task, int index) {
    QTableWidget* t = taskTable;
    if (index == -1) {
        index = t->rowCount();
    }

    t->insertRow(index);
    t->setItem(index, 0, new QTableWidgetItem(task->getName()));
    t->setItem(index, 1, new QTableWidgetItem(::ProcessState2StringHelper(task->getState())));
    t->setItem(index, 2, new QTableWidgetItem(task->getNextStartTime().toString()));
    t->setItem(index, 3, new QTableWidgetItem(task->getLastStartTime().toString()));
    t->setItem(index, 4, new QTableWidgetItem(task->getLastRunResult()));
}

static inline QString ProcessState2StringHelper(const QMap<QProcess::ProcessState, int>& states) {
    return QString("%1 Running, %2 Starting, %3 Not Running")
            .arg(states[QProcess::Running])
            .arg(states[QProcess::Starting])
            .arg(states[QProcess::NotRunning]);
}
