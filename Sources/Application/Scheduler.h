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
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QDir>
#include <QDebug>

#include "Plugins/TaskManagement/TaskManager.h"
#include "TaskCreationWizard.h"
#include "TaskSettingDialog.h"
#include "TaskTableWidget.h"
#include "GeneralSettingDialog.h"
#include "QuitOrHideDialog.h"

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
    void insertRow(Task* task, int index = -1);

    void updateRow(int index);

    void removeCurrentRow() {
        int current = taskTable->currentRow();
        if (current == -1) return;

        removeRow(current);
    }

private:
    void setupToolBar() {
        QAction* addAct = new QAction(QIcon(":/images/add.png"), "Add");
        connect(addAct, &QAction::triggered,
                &wizard, &TaskCreationWizard::show);

        QAction* removeAct = new QAction(QIcon(":/images/delete.png"), "Remove");
        connect(removeAct, &QAction::triggered,
                this,      &Scheduler::removeCurrentRow);

        QAction* settingAct = new QAction(QIcon(":/images/set.png"), "General Settings");
        connect(settingAct, &QAction::triggered,
                &settingDialog, &GeneralSettingDialog::show);

        toolBar = addToolBar("Tool Bar");
        toolBar->addAction(addAct);
        toolBar->addAction(removeAct);
        toolBar->addAction(settingAct);
    }

    void setupTaskTable() {
        taskTable = new TaskTableWidget(&taskManager, this);
        setCentralWidget(taskTable);
    }

    void setupSystemTray() {
        QSystemTrayIcon& s = systemTray;
        connect(&s,     &QSystemTrayIcon::activated,
                this,   &Scheduler::onSystemTrayActivated);
        s.setIcon(QIcon(":/images/scheduler.png"));
        s.setToolTip(kAppName);
        s.show();

        QAction* restoreWindowAct = new QAction("Restore Window", &s);
        connect(restoreWindowAct,   &QAction::triggered,
                this,               &Scheduler::showNormal);

        // TODO: open at boot in all platform.
        QAction* openAtBoot = new QAction("Open at boot", &s);
        openAtBoot->setCheckable(true);
        connect(openAtBoot, &QAction::triggered,
                this, &Scheduler::openAtBoot);

        QAction* quitAct = new QAction("Quit", &s);
        connect(quitAct, &QAction::triggered,
                qApp, &QApplication::quit);

        QMenu* menu = new QMenu();
        menu->addAction(restoreWindowAct);
        menu->addSeparator();
        menu->addAction(openAtBoot);
        menu->addAction(quitAct);
        s.setContextMenu(menu);
    }

    // TODO: close or hide.
    // should remeber the choice.
    void closeEvent(QCloseEvent* e) override {
//        QuitOrHideDialog dialog;

//        int retCode = dialog.exec();
//        if (retCode == QDialog::Accepted) {
//            e->accept();
//        } else {
//            e->ignore();
//            hide();
//        }

        e->accept();
    }

    void loadTasksFromTaskManager() {
        for (int i = 0; i != taskManager.size(); ++i) {
            Task* task = taskManager.get(i);
            insertRowWithoutChangeTaskManager(task);
        }
    }

    void insertRowWithoutChangeTaskManager(Task* task, int index = -1);

private slots:
    void openAtBoot(bool should) {
        const char* kWindowsRunPath = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
        QSettings reg(kWindowsRunPath, QSettings::NativeFormat);

        if (should) {
            QString appPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
            reg.setValue(kAppName, appPath);
        } else {
            reg.remove(kAppName);
        }
    }

    void onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
        case QSystemTrayIcon::Trigger:
            showNormal();
            break;

        default:
            break;
        }
    }

private:
    TaskManager taskManager;

    TaskCreationWizard wizard;
    QSystemTrayIcon systemTray;
    GeneralSettingDialog settingDialog;

    QToolBar* toolBar;
    QTableWidget* taskTable;

};

#endif // SCHEDULER_H
