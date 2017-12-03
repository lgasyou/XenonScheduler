#include "TaskTableWidget.h"

#include <QAction>
#include <QHeaderView>
#include <QMenu>
#include <QPoint>
#include <QDebug>

#include "Plugins/TaskManagement/TaskManager.h"
#include "TaskSettingDialog.h"

TaskTableWidget::TaskTableWidget(TaskManager* taskManager, QWidget* parent)
    : QTableWidget(parent),
      taskManager(taskManager)
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
    connect(showOptionAct, &QAction::triggered, [=]() {
        Task* task = taskManager->get(rowClicked);
        TaskSettingDialog* dialog = new TaskSettingDialog(task, this);
        dialog->show();
    });
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
