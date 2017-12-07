#include "OperationSettingDialog.h"
#include "Plugins/TaskManagement/Operation.h"

#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>

OperationSettingDialog::OperationSettingDialog(Operation* op, QWidget *parent) :
    QDialog(parent),
    operation(op)
{
    resize(800, 600);
    op != nullptr ? setupAsSettingDialog() : setupAsCreationDialog();
}

OperationSettingDialog::~OperationSettingDialog() {

}

void OperationSettingDialog::setupAsCreationDialog() {
    setWindowTitle("Creation");

    QLabel* taskLabel = new QLabel("Program or Script:");
    QLineEdit* taskEdit = new QLineEdit();

    QPushButton* taskBrowser = new QPushButton("Browser");
    QFileDialog* browerDialog = new QFileDialog(this);
    connect(taskBrowser, &QPushButton::clicked,
            browerDialog, &QFileDialog::show);
    connect(browerDialog, &QFileDialog::fileSelected,
            taskEdit, &QLineEdit::setText);

    QLabel* addArgLabel = new QLabel("Add Arguments(Optional):");
    QLineEdit* addArgEdit = new QLineEdit();

    QPushButton* acceptBtn = new QPushButton("Accept");
    connect(acceptBtn, &QPushButton::clicked, [=]() {
        operation = new Operation(taskEdit->text(), QStringList() << addArgEdit->text());
        accept();
    });
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked,
            this, &OperationSettingDialog::reject);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(taskLabel, 0, 0);
    layout->addWidget(taskEdit, 1, 0, 1, 2);
    layout->addWidget(taskBrowser, 1, 2, 1, 1);
    layout->addWidget(addArgLabel, 2, 0, 1, 1);
    layout->addWidget(addArgEdit, 2, 1, 1, 2);
    layout->addWidget(acceptBtn, 3, 0);
    layout->addWidget(cancelBtn, 3, 1);
}

void OperationSettingDialog::setupAsSettingDialog() {
    setWindowTitle("Settings");

    QLabel* taskLabel = new QLabel("Program or Script:");
    QLineEdit* taskEdit = new QLineEdit(operation->program);

    QPushButton* taskBrowser = new QPushButton("Browser");
    QFileDialog* browerDialog = new QFileDialog(this);
    connect(taskBrowser, &QPushButton::clicked,
            browerDialog, &QFileDialog::show);
    connect(browerDialog, &QFileDialog::fileSelected,
            taskEdit, &QLineEdit::setText);

    QLabel* addArgLabel = new QLabel("Add Arguments(Optional):");
    QLineEdit* addArgEdit = new QLineEdit(operation->arguments.join(' '));

    QPushButton* acceptBtn = new QPushButton("Accept");
    connect(acceptBtn, &QPushButton::clicked, [=]() {
        operation->arguments = QStringList() << addArgEdit->text();
        operation->program = taskEdit->text();
        accept();
    });
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked,
            this, &OperationSettingDialog::reject);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(taskLabel, 0, 0);
    layout->addWidget(taskEdit, 1, 0, 1, 2);
    layout->addWidget(taskBrowser, 1, 2, 1, 1);
    layout->addWidget(addArgLabel, 2, 0, 1, 1);
    layout->addWidget(addArgEdit, 2, 1, 1, 2);
    layout->addWidget(acceptBtn, 3, 0);
    layout->addWidget(cancelBtn, 3, 1);
}
