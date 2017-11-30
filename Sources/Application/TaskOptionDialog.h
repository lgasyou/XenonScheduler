#ifndef TASKOPTIONDIALOG_H
#define TASKOPTIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QHeaderView>
#include <QTableWidget>
#include <QPushButton>
#include <QStringList>
#include <QDebug>

#include "ui_TaskOptionDialog.h"
#include "Plugins/TaskManagement/Task.h"

namespace Ui {
class TaskOptionDialog;
}

class TaskOptionDialog : public QDialog {
    Q_OBJECT

    const QStringList kTriggerTableLabels = {
        "Trigger", "Detail", "State",
    };
    const QStringList kOperationTableLabels = {
        "Operation", "Detail",
    };
    const QStringList kJournalTableLabels = {
        "Level", "Date",
    };

public:
    explicit TaskOptionDialog::TaskOptionDialog(QWidget *parent = 0) :
        QDialog(parent),
        ui(new Ui::TaskOptionDialog)
    {
        ui->setupUi(this);
        resize(800, 600);

        setupGeneralTab();
        setupTriggerTab();
        setupOperationTab();
        setupJournalTab();
    }

    ~TaskOptionDialog() {
        delete ui;
    }

public slots:
    void showWithTaskOptions(Task* task) {
        setWindowTitle(task->getName() + " - Properties");

        nameEdit->setText(task->getName());
        nameEdit->setDisabled(true);
        locationEdit->setText("\\");
        locationEdit->setDisabled(true);
        briefEdit->setPlainText(task->getDescription());

        // TODO: add triggers and operations.

        show();
    }

private:
    void setupGeneralTab() {
        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout();
        w->setLayout(l);
        ui->tabWidget->addTab(w, "General");

        l->addWidget(new QLabel("Name:"), 0, 0);
        nameEdit = new QLineEdit();
        l->addWidget(nameEdit, 0, 1);
        l->addWidget(new QLabel("Location:"), 1, 0);
        locationEdit = new QLineEdit();
        l->addWidget(locationEdit, 1, 1);
        l->addWidget(new QLabel("Description:"), 2, 0);
        briefEdit = new QPlainTextEdit();
        l->addWidget(briefEdit, 2, 1);
    }

    void setupTriggerTab() {
        QTableWidget* table = new QTableWidget();
        table->setColumnCount(kTriggerTableLabels.size());
        table->setHorizontalHeaderLabels(kTriggerTableLabels);
        table->verticalHeader()->hide();
        table->setRowCount(1);

        QPushButton* newBtn = new QPushButton("New...");
        QPushButton* editBtn = new QPushButton("Edit...");
        QPushButton* removeBtn = new QPushButton("Remove...");

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout();
        l->addWidget(table, 0, 0, 1, 6);
        l->addWidget(newBtn, 1, 0);
        l->addWidget(editBtn, 1, 1);
        l->addWidget(removeBtn, 1, 2);
        w->setLayout(l);
        ui->tabWidget->addTab(w, "Triggers");
    }

    void setupOperationTab() {
        QTableWidget* table = new QTableWidget();
        table->setColumnCount(kOperationTableLabels.size());
        table->setHorizontalHeaderLabels(kOperationTableLabels);
        table->verticalHeader()->hide();
        table->setRowCount(1);

        QPushButton* newBtn = new QPushButton("New...");
        QPushButton* editBtn = new QPushButton("Edit...");
        QPushButton* removeBtn = new QPushButton("Remove...");

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout();
        l->addWidget(table, 0, 0, 1, 6);
        l->addWidget(newBtn, 1, 0);
        l->addWidget(editBtn, 1, 1);
        l->addWidget(removeBtn, 1, 2);
        w->setLayout(l);
        ui->tabWidget->addTab(w, "Opertions");
        triggerTable = table;
    }

    void setupJournalTab() {
        QTableWidget* table = new QTableWidget();
        table->setColumnCount(kJournalTableLabels.size());
        table->setHorizontalHeaderLabels(kJournalTableLabels);
        table->verticalHeader()->hide();
        table->setRowCount(1);

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout();
        l->addWidget(table, 0, 0, 1, 3);
        w->setLayout(l);
        ui->tabWidget->addTab(w, "Journal");
        operationTable = table;
    }

    void closeEvent(QCloseEvent* event) override {
        deleteLater();
        QDialog::closeEvent(event);
    }

private:
    Ui::TaskOptionDialog *ui;

    QLineEdit* nameEdit;
    QLineEdit* locationEdit;
    QPlainTextEdit* briefEdit;

    QTableWidget* triggerTable;
    QTableWidget* operationTable;
};

#endif // TASKOPTIONDIALOG_H
