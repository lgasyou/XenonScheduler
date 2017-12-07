#ifndef CREATETRIGGERDIALOG_H
#define CREATETRIGGERDIALOG_H

#include <QDialog>

class Trigger;

class TriggerSettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit TriggerSettingDialog(Trigger* trigger, QWidget *parent = 0);
    ~TriggerSettingDialog() {}

    Trigger* getTrigger() const {
        return trigger;
    }

private:
    void setupAsCreationDialog();

    void setupAsSettingDialog();

private:
    Trigger* trigger;

};

#endif // CREATETRIGGERDIALOG_H
