#include "TaskTableWidget.h"

#include <QAction>
#include <QHeaderView>
#include <QMenu>
#include <QPoint>
#include <QDebug>

#include "Plugins/TaskManagement/TaskManager.h"
#include "TaskOptionDialog.h"

TaskTableWidget::TaskTableWidget(TaskManager* taskManager, QWidget* parent)
    : QTableWidget(parent),
      taskManager(taskManager)
{
    setColumnCount(kTableLabels.size());
    setHorizontalHeaderLabels(kTableLabels);
    verticalHeader()->hide();

    setSelectionBehavior(QTableWidget::SelectRows);
    setSelectionMode(QTableWidget::SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSortingEnabled(true);

    connect(this, &TaskTableWidget::customContextMenuRequested,
            this, &TaskTableWidget::onRightMouseClicked);

    popMenu = new QMenu(this);

    startAct = new QAction("Start");
    stopAct = new QAction("Stop");
    showOptionAct = new QAction("Option");

    connect(startAct, &QAction::triggered,
            [=]() { taskManager->runTaskAt(rowClicked); });
    connect(stopAct, &QAction::triggered,
            [=]() { taskManager->stopTaskAt(rowClicked); });
    connect(showOptionAct, &QAction::triggered, [=]() {
        Task* task = taskManager->get(rowClicked);
        TaskOptionDialog* dialog = new TaskOptionDialog(this);
        dialog->showWithTaskOptions(task);
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
