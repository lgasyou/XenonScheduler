#include "TaskSettingDialog.h"

static QString IntervalType2StringHelper(Trigger::IntervalType type);

void TaskSettingDialog::showEvent(QShowEvent *e) {
    setWindowTitle(task->getName() + " - Properties");

    nameEdit->setText(task->getName());
    nameEdit->setDisabled(true);
    locationEdit->setText("\\");
    locationEdit->setDisabled(true);
    briefEdit->setPlainText(task->getDescription());

    updateTriggerTable();
    updateOperationTable();

    QDialog::showEvent(e);
}

void TaskSettingDialog::updateTriggerTable() {
    const QVector<Trigger*>& triggers = task->getTriggers();
    triggerTable->setRowCount(triggers.size());
    for (int i = 0, size = triggers.size(); i != size; ++i) {
        Trigger* t = triggers[i];
        triggerTable->setItem(i, 0, new QTableWidgetItem("Every " + ::IntervalType2StringHelper(t->intervalType)));
        triggerTable->setItem(i, 1, new QTableWidgetItem(QString("Every %1").arg(t->interval)));
        triggerTable->setItem(i, 2, new QTableWidgetItem("Activated"));
    }
    triggerTable->selectRow(0);
}

void TaskSettingDialog::updateOperationTable() {
    const QVector<Operation*>& operations = task->getOperations();
    operationTable->setRowCount(operations.size());
    for (int i = 0, size = operations.size(); i != size; ++i) {
        Operation* o = operations[i];
        operationTable->setItem(i, 0, new QTableWidgetItem(o->program));
        operationTable->setItem(i, 1, new QTableWidgetItem(o->arguments.join(' ')));
    }
    operationTable->selectRow(0);
}

static inline QString IntervalType2StringHelper(Trigger::IntervalType type) {
    // Use map is safer than use array.
    static QMap<Trigger::IntervalType, QString> map = {
        { Trigger::Day, "Day" },
        { Trigger::Week, "Week" },
        { Trigger::Month, "Month" },
        { Trigger::Once, "Once" },
        { Trigger::StartingUp, "StartingUp" },
        { Trigger::Login, "Login" },
    };
    return map[type];
}
