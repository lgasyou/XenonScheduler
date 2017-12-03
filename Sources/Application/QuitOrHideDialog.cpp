#include "QuitOrHideDialog.h"
#include <QPushButton>
#include <QHBoxLayout>

QuitOrHideDialog::QuitOrHideDialog(QWidget *parent)
    : QDialog(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    QPushButton* quitBtn = new QPushButton("Quit");
    connect(quitBtn, &QPushButton::clicked,
            this, &QuitOrHideDialog::accept);

    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked,
            this, &QuitOrHideDialog::close);

    QPushButton* hideBtn = new QPushButton("Hide");
    connect(hideBtn, &QPushButton::clicked,
            this, &QuitOrHideDialog::reject);

    layout->addWidget(quitBtn);
    layout->addWidget(cancelBtn);
    layout->addWidget(hideBtn);
}
