#include "QuitOrHideDialog.h"
#include <QPushButton>
#include <QHBoxLayout>

QuitOrHideDialog::QuitOrHideDialog(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    QPushButton* quitBtn = new QPushButton("Quit");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QPushButton* hideBtn = new QPushButton("Hide");

    layout->addWidget(quitBtn);
    layout->addWidget(cancelBtn);
    layout->addWidget(hideBtn);
}
