#ifndef EDITTRIGGERDIALOG_H
#define EDITTRIGGERDIALOG_H

#include <QDialog>

namespace Ui {
class EditTriggerDialog;
}

class EditTriggerDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditTriggerDialog(QWidget *parent = 0);
    ~EditTriggerDialog();

private:
    Ui::EditTriggerDialog *ui;
};

#endif // EDITTRIGGERDIALOG_H
