#ifndef EDITOPERATIONDIALOG_H
#define EDITOPERATIONDIALOG_H

#include <QDialog>

namespace Ui {
class EditOperationDialog;
}

class EditOperationDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditOperationDialog(QWidget *parent = 0);
    ~EditOperationDialog();

private:
    Ui::EditOperationDialog *ui;
};

#endif // EDITOPERATIONDIALOG_H
