#include "EditOperationDialog.h"
#include "ui_EditOperationDialog.h"

EditOperationDialog::EditOperationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditOperationDialog)
{
    ui->setupUi(this);
}

EditOperationDialog::~EditOperationDialog() {
    delete ui;
}
