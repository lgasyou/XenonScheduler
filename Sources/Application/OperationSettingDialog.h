#ifndef CREATEOPERATIONDIALOG_H
#define CREATEOPERATIONDIALOG_H

#include <QDialog>

class Operation;

class OperationSettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit OperationSettingDialog(Operation* op, QWidget *parent = 0);
    ~OperationSettingDialog();

    Operation* getOperation() const {
        return operation;
    }

private:
    void setupAsCreationDialog();

    void setupAsSettingDialog();

private:
    Operation* operation;

};

#endif // CREATEOPERATIONDIALOG_H
