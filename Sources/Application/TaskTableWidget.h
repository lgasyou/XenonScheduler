#ifndef TASKTABLEWIDGET_H
#define TASKTABLEWIDGET_H

#include <QTableWidget>

class QMenu;
class QAction;
class Scheduler;
class TaskManager;

class TaskTableWidget : public QTableWidget {
    const QStringList kTableLabels = {
        "Task",
        "State",
        "Next Start Time",
        "Last Run Time",
        "Last Run Result",
    };

public:
    TaskTableWidget(TaskManager* taskManager, Scheduler* scheduler);

private:
    void onRightMouseClicked(const QPoint& point);

private slots:
    void showTaskSetting();

private:
    TaskManager* taskManager;
    Scheduler* scheduler;

    QMenu* popMenu;
    QAction* startAct;
    QAction* stopAct;
    QAction* showOptionAct;

    int rowClicked;
};

#endif // TASKTABLEWIDGET_H
