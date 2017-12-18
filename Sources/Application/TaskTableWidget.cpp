#include "TaskTableWidget.h"

#include <QAction>
#include <QHeaderView>
#include <QMenu>
#include <QPoint>
#include <QDebug>

#include "Plugins/TaskManagement/TaskManager.h"
#include "Plugins/TaskManagement/Task.h"
#include "TaskSettingDialog.h"
#include "Scheduler.h"

TaskTableWidget::TaskTableWidget(TaskManager* taskManager, Scheduler* scheduler)
    : QTableWidget(scheduler),
      taskManager(taskManager),
      scheduler(scheduler)
{
    setColumnCount(kTableLabels.size());
    setHorizontalHeaderLabels(kTableLabels);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    verticalHeader()->hide();

    setSelectionBehavior(QTableWidget::SelectRows);
    setSelectionMode(QTableWidget::SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QTableWidget::NoEditTriggers);
    setSortingEnabled(true);

    connect(this, &TaskTableWidget::customContextMenuRequested,
            this, &TaskTableWidget::onRightMouseClicked);

    popMenu = new QMenu(this);

    startAct = new QAction("Start");
    stopAct = new QAction("Stop");
    showOptionAct = new QAction("Option");

    connect(startAct, &QAction::triggered,
            [=]() { taskManager->startTaskAt(rowClicked); });
    connect(stopAct, &QAction::triggered,
            [=]() { taskManager->stopTaskAt(rowClicked); });
    connect(showOptionAct, &QAction::triggered,
            this, &TaskTableWidget::showTaskSetting);
}

void TaskTableWidget::onRightMouseClicked(const QPoint& point) {
    QTableWidgetItem *item = itemAt(point);
    if (item) {
        popMenu->clear();
        popMenu->addAction(startAct);
        popMenu->addAction(stopAct);
        popMenu->addAction(showOptionAct);

        rowClicked = item->row();
        popMenu->exec(QCursor::pos());
    }
}

void TaskTableWidget::showTaskSetting() {
    Task* task = taskManager->get(rowClicked);
    TaskSettingDialog* dialog = new TaskSettingDialog(task, this);
    connect(dialog, &TaskSettingDialog::taskChanged, [=](Task* task) {
        int index = taskManager->indexOf(task);
        scheduler->updateRow(index);
    });
    dialog->show();
}
