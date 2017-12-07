#include "TriggerSettingDialog.h"

TriggerSettingDialog::TriggerSettingDialog(Trigger* trigger, QWidget *parent) :
    QDialog(parent),
    trigger(trigger)
{
    setAttribute(Qt::WA_DeleteOnClose);
    resize(800, 600);
    trigger != nullptr ? setupAsSettingDialog() : setupAsCreationDialog();
}

TriggerSettingDialog::~TriggerSettingDialog() {

}

void TriggerSettingDialog::setupAsCreationDialog() {
    setWindowTitle("Creation");
}

void TriggerSettingDialog::setupAsSettingDialog() {
    setWindowTitle("Settings");
}
