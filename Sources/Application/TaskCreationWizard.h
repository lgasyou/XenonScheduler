#ifndef CREATETASKWIZARD_H
#define CREATETASKWIZARD_H

#include <QWizard>
#include <QVariant>
#include <QDateTime>

#include "Plugins/TaskManagement/TaskManager.h"

class Scheduler;

class TaskCreationWizard : public QWizard {
public:
    enum PageId {
        BasicPage,
        SelectingTriggerPage,
        DailyTriggerPage,
        WeeklyTriggerPage,
        MonthlyTriggerPage,
        OnceTriggerPage,
        StartingUpTriggerPage,
        LoginTriggerPage,
        OperationPage,
        FinishingPage,
    };

    TaskCreationWizard(TaskManager* taskManager, Scheduler* scheduler = 0);

private:
    void accept() override;

private:
    TaskManager* taskManager;
    Scheduler* scheduler;
};

#endif // CREATETASKWIZARD_H
