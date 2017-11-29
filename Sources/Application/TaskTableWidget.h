#ifndef TASKTABLEWIDGET_H
#define TASKTABLEWIDGET_H

#include <QTableWidget>

class QMenu;
class QAction;
class TaskManager;

class TaskTableWidget : public QTableWidget {
    const QStringList kTableLabels = {
        "Script",
        "State",
        "Next Start Time",
        "Last Run Time",
        "Last Run Result",
    };

public:
    TaskTableWidget(TaskManager* taskManager);

private:
    void onRightMouseClicked(const QPoint& point);

private:
    TaskManager* taskManager;

    QMenu* popMenu;
    QAction* startAct;
    QAction* stopAct;
    QAction* showOptionAct;
};

#endif // TASKTABLEWIDGET_H
