#ifndef CREATEOPERATIONDIALOG_H
#define CREATEOPERATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateOperationDialog;
}

class CreateOperationDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreateOperationDialog(QWidget *parent = 0);
    ~CreateOperationDialog();

private:
    Ui::CreateOperationDialog *ui;
};

#endif // CREATEOPERATIONDIALOG_H
