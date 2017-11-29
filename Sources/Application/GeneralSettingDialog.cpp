#include "GeneralSettingDialog.h"
#include "ui_GeneralSettingDialog.h"

GeneralSettingDialog::GeneralSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingDialog)
{
    ui->setupUi(this);
}

GeneralSettingDialog::~GeneralSettingDialog()
{
    delete ui;
}
