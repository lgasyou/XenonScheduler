#include "CreateOperationDialog.h"
#include "ui_CreateOperationDialog.h"

CreateOperationDialog::CreateOperationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateOperationDialog)
{
    ui->setupUi(this);
}

CreateOperationDialog::~CreateOperationDialog() {
    delete ui;
}
