#include "EditTriggerDialog.h"
#include "ui_EditTriggerDialog.h"

EditTriggerDialog::EditTriggerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTriggerDialog)
{
    ui->setupUi(this);
}

EditTriggerDialog::~EditTriggerDialog()
{
    delete ui;
}
