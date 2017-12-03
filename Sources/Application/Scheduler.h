#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QMainWindow>
#include <QTableWidget>
#include <QSystemTrayIcon>
#include <QStringList>
#include <QToolBar>
#include <QMenu>
#include <QPushButton>
#include <QHeaderView>
#include <QDebug>

#include "Plugins/TaskManagement/TaskManager.h"
#include "CreateTaskWizard.h"
#include "GeneralSettingDialog.h"
#include "TaskOptionDialog.h"
#include "TaskTableWidget.h"

namespace Ui {
class Scheduler;
}

class Scheduler : public QMainWindow {
    Q_OBJECT

    const QString kAppName = "Xenon Scheduler";

public:
    explicit Scheduler(QWidget *parent = 0);
    ~Scheduler();

    void removeRow(int index) {
        taskManager.remove(index);
        taskTable->removeRow(index);
    }

public slots:
    // if index is -1, then add at the last position.
    void insertRow(Task* task, int index = -1) {
        QTableWidget* t = taskTable;
        if (index == -1) {
            index = t->rowCount();
        }

        t->insertRow(index);
        taskManager.insert(task, index);

        t->setItem(index, 0, new QTableWidgetItem(task->getName()));
        t->setItem(index, 1, new QTableWidgetItem(task->getState()));
        t->setItem(index, 2, new QTableWidgetItem(task->getNextStartTime().toString()));
        t->setItem(index, 3, new QTableWidgetItem(task->getLastStartTime().toString()));
        t->setItem(index, 4, new QTableWidgetItem(task->getLastRunResult()));
    }

    void updateRow(int index) {
        qDebug() << "Scheduler::updateRow(int)";
        Task* task = taskManager.get(index);
        taskTable->setItem(index, 0, new QTableWidgetItem(task->getName()));
        taskTable->setItem(index, 1, new QTableWidgetItem(task->getState()));
        taskTable->setItem(index, 2, new QTableWidgetItem(task->getNextStartTime().toString()));
        taskTable->setItem(index, 3, new QTableWidgetItem(task->getLastStartTime().toString()));
        taskTable->setItem(index, 4, new QTableWidgetItem(task->getLastRunResult()));
    }

    void removeCurrentRow() {
        int current = taskTable->currentRow();
        if (current == -1) return;

        removeRow(current);
    }

private:
    void setupToolBar() {
        QAction* addAct = new QAction(QIcon(":/images/add.png"), "Add");
        wizard = new CreateTaskWizard(&taskManager, this);
        connect(addAct, &QAction::triggered,
                wizard, &CreateTaskWizard::show);

        QAction* removeAct = new QAction(QIcon(":/images/delete.png"), "Remove");
        connect(removeAct, &QAction::triggered,
                this,      &Scheduler::removeCurrentRow);

        QAction* settingAct = new QAction(QIcon(":/images/set.png"), "General Settings");
        settingDialog = new GeneralSettingDialog(this);
        connect(settingAct, &QAction::triggered,
                settingDialog, &GeneralSettingDialog::show);

        toolBar = addToolBar("Tool Bar");
        toolBar->addAction(addAct);
        toolBar->addAction(removeAct);
        toolBar->addAction(settingAct);
    }

    void setupTaskTableWidget() {
        QTableWidget* t = new TaskTableWidget(&taskManager);
        setCentralWidget(t);
        taskTable = t;
    }

    void setupSystemTray() {
        QSystemTrayIcon* s = new QSystemTrayIcon(this);
        s->setIcon(QIcon(":/images/task-management.png"));
        s->setToolTip(kAppName);
        s->show();

        QAction* restoreWindowAct = new QAction("Restore Window", s);
        QAction* quitAct = new QAction("Quit", s);

        QMenu* menu = new QMenu();
        menu->addAction(restoreWindowAct);
        menu->addAction(quitAct);
        s->setContextMenu(menu);
        systemTray = s;
    }

    // TODO: close or hide.
    // should remeber the choice.
    void closeEvent(QCloseEvent* e) override {
        QMainWindow::closeEvent(e);
    }

private:
    TaskManager taskManager;
    QToolBar* toolBar;
    QTableWidget* taskTable;
    QSystemTrayIcon* systemTray;
    CreateTaskWizard* wizard;
    GeneralSettingDialog* settingDialog;

};

#endif // SCHEDULER_H
