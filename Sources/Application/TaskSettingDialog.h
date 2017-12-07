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
    void showEvent(QShowEvent* e) override;

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
            TriggerSettingDialog* dialog = new TriggerSettingDialog(nullptr, this);
            if (dialog->exec() == QDialog::Accepted) {
                updateTriggerTable();
            }
        });

        QPushButton* editBtn = new QPushButton("Edit...");
        connect(editBtn, &QPushButton::clicked, [=]() {
            int selected = table->currentRow();
            if (selected != -1) {
                Trigger* trigger = task->getTriggerAt(selected);
                TriggerSettingDialog* dialog = new TriggerSettingDialog(trigger, this);
                if (dialog->exec() == QDialog::Accepted) {
                    updateTriggerTable();
                }
            }
        });

        QPushButton* removeBtn = new QPushButton("Remove...");
        connect(removeBtn, &QPushButton::clicked, [=]() {
            int selected = table->currentRow();
            if (selected != -1) {
                task->removeTriggerAt(selected);
                updateTriggerTable();
            }
        });

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout(w);
        l->addWidget(table, 0, 0, 1, 6);
        l->addWidget(newBtn, 1, 0);
        l->addWidget(editBtn, 1, 1);
        l->addWidget(removeBtn, 1, 2);
        tabWidget->addTab(w, "Triggers");
        triggerTable = table;
    }

    void setupOperationTab() {
        QTableWidget* table = new QTableWidget();
        table->setColumnCount(kOperationTableLabels.size());
        table->setHorizontalHeaderLabels(kOperationTableLabels);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table->verticalHeader()->hide();
        table->setSelectionBehavior(QTableWidget::SelectRows);
        table->setSelectionMode(QTableWidget::SingleSelection);
        table->setContextMenuPolicy(Qt::CustomContextMenu);
        table->setEditTriggers(QTableWidget::NoEditTriggers);
        table->setSortingEnabled(true);

        QPushButton* newBtn = new QPushButton("New...");
        connect(newBtn, &QPushButton::clicked, [=]() {
            OperationSettingDialog* dialog = new OperationSettingDialog(nullptr, this);
            if (dialog->exec() == QDialog::Accepted) {
                updateOperationTable();
            }
        });

        QPushButton* editBtn = new QPushButton("Edit...");
        connect(editBtn, &QPushButton::clicked, [=]() {
            int selected = table->currentRow();
            if (selected != -1) {
                Operation* op = task->getOperationAt(selected);
                OperationSettingDialog* dialog = new OperationSettingDialog(op, this);
                if (dialog->exec() == QDialog::Accepted) {
                    updateOperationTable();
                }
            }
        });

        QPushButton* removeBtn = new QPushButton("Remove...");
        connect(removeBtn, &QPushButton::clicked, [=]() {
            int selected = table->currentRow();
            if (selected != -1) {
                task->removeOperationAt(selected);
                updateOperationTable();
            }
        });

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout(w);
        l->addWidget(table, 0, 0, 1, 6);
        l->addWidget(newBtn, 1, 0);
        l->addWidget(editBtn, 1, 1);
        l->addWidget(removeBtn, 1, 2);
        tabWidget->addTab(w, "Opertions");
        operationTable = table;
    }

    void setupJournalTab() {
        QTableWidget* table = new QTableWidget();
        table->setColumnCount(kJournalTableLabels.size());
        table->setHorizontalHeaderLabels(kJournalTableLabels);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table->verticalHeader()->hide();

        QWidget* w = new QWidget();
        QGridLayout* l = new QGridLayout(w);
        l->addWidget(table, 0, 0, 1, 3);
        tabWidget->addTab(w, "Journal");
        journalTable = table;
    }

    void updateTriggerTable();

    void updateOperationTable();

private:
    Task* task;

    QTabWidget* tabWidget;

    QLineEdit* nameEdit;
    QLineEdit* locationEdit;
    QPlainTextEdit* briefEdit;

    QTableWidget* triggerTable;
    QTableWidget* operationTable;
    QTableWidget* journalTable;
};

#endif // TASKOPTIONDIALOG_H
