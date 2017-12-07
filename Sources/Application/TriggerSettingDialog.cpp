#include "TriggerSettingDialog.h"
#include "Plugins/TaskManagement/Trigger.h"

#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

TriggerSettingDialog::TriggerSettingDialog(Trigger* trigger, QWidget *parent) :
    QDialog(parent),
    trigger(trigger)
{
    resize(800, 600);
    trigger != nullptr ? setupAsSettingDialog() : setupAsCreationDialog();
}

// TODO: setupAsCreationDialog
void TriggerSettingDialog::setupAsCreationDialog() {
    setWindowTitle("Creation");

    QGridLayout* layout = new QGridLayout(this);
    QLabel* startWhenLabel = new QLabel("Start When: ");
    QComboBox* comboBox = new QComboBox();

    QCheckBox* activatedCheckBox = new QCheckBox("Activated");

    QPushButton* acceptBtn = new QPushButton("Accept");
    connect(acceptBtn, &QPushButton::clicked, [=]() {
//        trigger = new Trigger();
        accept();
    });
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked,
            this, &TriggerSettingDialog::reject);

    layout->addWidget(startWhenLabel, 0, 0);
    layout->addWidget(comboBox, 0, 1);
    layout->addWidget(activatedCheckBox, 1, 0);
    layout->addWidget(acceptBtn, 2, 0);
    layout->addWidget(cancelBtn, 2, 1);
}

// TODO: setupAsSettingDialog
void TriggerSettingDialog::setupAsSettingDialog() {
    setWindowTitle("Settings");

    QGridLayout* layout = new QGridLayout(this);
    QLabel* startWhenLabel = new QLabel("Start When: ");
    QComboBox* comboBox = new QComboBox();

    QCheckBox* activatedCheckBox = new QCheckBox("Activated");

    QPushButton* acceptBtn = new QPushButton("Accept");
    connect(acceptBtn, &QPushButton::clicked, [=]() {
        accept();
    });
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked,
            this, &TriggerSettingDialog::reject);

    layout->addWidget(startWhenLabel, 0, 0);
    layout->addWidget(comboBox, 0, 1);
    layout->addWidget(activatedCheckBox, 1, 0);
    layout->addWidget(acceptBtn, 2, 0);
    layout->addWidget(cancelBtn, 2, 1);
}
