#include "OperationSettingDialog.h"
#include "Plugins/TaskManagement/Operation.h"

OperationSettingDialog::OperationSettingDialog(Operation* op, QWidget *parent) :
    QDialog(parent),
    operation(op)
{
    setAttribute(Qt::WA_DeleteOnClose);
    resize(800, 600);
    op != nullptr ? setupAsSettingDialog() : setupAsCreationDialog();
}

OperationSettingDialog::~OperationSettingDialog() {

}

void OperationSettingDialog::setupAsCreationDialog() {
    setWindowTitle("Creation");
}

void OperationSettingDialog::setupAsSettingDialog() {
    setWindowTitle("Settings");
}
