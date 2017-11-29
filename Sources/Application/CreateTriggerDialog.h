#ifndef CREATETRIGGERDIALOG_H
#define CREATETRIGGERDIALOG_H

#include <QDialog>

namespace Ui {
class CreateTriggerDialog;
}

class CreateTriggerDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreateTriggerDialog(QWidget *parent = 0);
    ~CreateTriggerDialog();

private:
    Ui::CreateTriggerDialog *ui;
};

#endif // CREATETRIGGERDIALOG_H
