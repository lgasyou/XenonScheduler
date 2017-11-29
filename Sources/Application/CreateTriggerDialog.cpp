#include "CreateTriggerDialog.h"
#include "ui_CreateTriggerDialog.h"

CreateTriggerDialog::CreateTriggerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTriggerDialog)
{
    ui->setupUi(this);
}

CreateTriggerDialog::~CreateTriggerDialog()
{
    delete ui;
}
