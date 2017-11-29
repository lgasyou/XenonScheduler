#ifndef CREATETASKWIZARD_H
#define CREATETASKWIZARD_H

#include <QWizard>
#include <QVariant>
#include <QDateTime>

#include "Plugins/TaskManager.h"

class Scheduler;

class CreateTaskWizard : public QWizard {
public:
    static const QStringList kInternalChoices;

    CreateTaskWizard(TaskManager* taskManager, Scheduler* scheduler = 0);

private:
    void accept() override;

private:
    TaskManager* taskManager;
    Scheduler* scheduler;
};

#endif // CREATETASKWIZARD_H
