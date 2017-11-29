#include "TaskTableWidget.h"

#include <QAction>
#include <QHeaderView>
#include <QMenu>
#include <QPoint>
#include <QDebug>

#include "Plugins/TaskManager.h"
#include "TaskOptionDialog.h"

TaskTableWidget::TaskTableWidget(TaskManager* taskManager)
    : taskManager(taskManager)
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
            [index, this]() { taskManager.runTaskAt(index); });
    connect(stopAct, &QAction::triggered,
            [index, this]() { taskManager.stopTaskAt(index); });
    TaskOptionDialog* tod = new TaskOptionDialog(task, this);
    connect(showOptionAct, &QAction::triggered,
            tod, &TaskOptionDialog::show);
}

void TaskTableWidget::onRightMouseClicked(const QPoint& point) {
    popMenu->clear();

    QTableWidgetItem *item = itemAt(point);
    if (item) {
        popMenu->addAction(startAct);
        popMenu->addAction(stopAct);
        popMenu->addAction(showOptionAct);

        popMenu->exec(QCursor::pos());
    }
}
