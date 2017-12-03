#ifndef TASKOPTIONDIALOG_H
#define TASKOPTIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QHeaderView>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QStringList>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>

#include "Plugins/TaskManagement/Task.h"
#include "OperationSettingDialog.h"
#include "TriggerSettingDialog.h"

class TaskSettingDialog : public QDialog {
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
    explicit TaskSettingDialog::TaskSettingDialog(Task* task, QWidget *parent = 0) :
        QDialog(parent),
        task(task),
        tabWidget(new QTabWidget())
    {
        resize(800, 600);
        setAttribute(Qt::WA_DeleteOnClose);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->addWidget(tabWidget);

        setupGeneralTab();
        setupTriggerTab();
        setupOperationTab();
        setupJournalTab();
    }

    ~TaskSettingDialog() = default;

private:
    void showEvent(QShowEvent* e) override {
        setWindowTitle(task->getName() + " - Properties");

        nameEdit->setText(task->getName());
        nameEdit->setDisabled(true);
        locationEdit->setText("\\");
        locationEdit->setDisabled(true);
        briefEdit->setPlainText(task->getDescription());

        // TODO: add triggers and operations.

        QDialog::showEvent(e);
    }

    void setupGeneralTab() {
        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout(w);
        tabWidget->addTab(w, "General");

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
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table->verticalHeader()->hide();
        table->setSelectionBehavior(QTableWidget::SelectRows);
        table->setSelectionMode(QTableWidget::SingleSelection);
        table->setContextMenuPolicy(Qt::CustomContextMenu);
        table->setEditTriggers(QTableWidget::NoEditTriggers);
        table->setSortingEnabled(true);

        QPushButton* newBtn = new QPushButton("New...");
        connect(newBtn, &QPushButton::clicked, [=]() {
            // TODO: Trigger setting.
//            int selected = table->currentRow();
            TriggerSettingDialog* dialog = new TriggerSettingDialog(this);
            dialog->show();
        });

        QPushButton* editBtn = new QPushButton("Edit...");

        QPushButton* removeBtn = new QPushButton("Remove...");

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout(w);
        l->addWidget(table, 0, 0, 1, 6);
        l->addWidget(newBtn, 1, 0);
        l->addWidget(editBtn, 1, 1);
        l->addWidget(removeBtn, 1, 2);
        tabWidget->addTab(w, "Triggers");
    }

    void setupOperationTab() {
        QTableWidget* table = new QTableWidget();
        table->setColumnCount(kTriggerTableLabels.size());
        table->setHorizontalHeaderLabels(kTriggerTableLabels);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table->verticalHeader()->hide();
        table->setSelectionBehavior(QTableWidget::SelectRows);
        table->setSelectionMode(QTableWidget::SingleSelection);
        table->setContextMenuPolicy(Qt::CustomContextMenu);
        table->setEditTriggers(QTableWidget::NoEditTriggers);
        table->setSortingEnabled(true);

        QPushButton* newBtn = new QPushButton("New...");
        connect(newBtn, &QPushButton::clicked, [=]() {
            // TODO: operation setting.
//            int selected = table->currentRow();
            OperationSettingDialog* dialog = new OperationSettingDialog(this);
            dialog->show();
        });

        QPushButton* editBtn = new QPushButton("Edit...");
        QPushButton* removeBtn = new QPushButton("Remove...");

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout(w);
        l->addWidget(table, 0, 0, 1, 6);
        l->addWidget(newBtn, 1, 0);
        l->addWidget(editBtn, 1, 1);
        l->addWidget(removeBtn, 1, 2);
        tabWidget->addTab(w, "Opertions");
        triggerTable = table;
    }

    void setupJournalTab() {
        QTableWidget* table = new QTableWidget();
        table->setColumnCount(kJournalTableLabels.size());
        table->setHorizontalHeaderLabels(kJournalTableLabels);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table->verticalHeader()->hide();
        table->setRowCount(1);

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout(w);
        l->addWidget(table, 0, 0, 1, 3);
        tabWidget->addTab(w, "Journal");
        operationTable = table;
    }

private:
    Task* task;

    QTabWidget* tabWidget;

    QLineEdit* nameEdit;
    QLineEdit* locationEdit;
    QPlainTextEdit* briefEdit;

    QTableWidget* triggerTable;
    QTableWidget* operationTable;
};

#endif // TASKOPTIONDIALOG_H
