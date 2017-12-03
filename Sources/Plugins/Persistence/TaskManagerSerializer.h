#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QString>
#include <QFile>

#include "Plugins/TaskManagement/TaskManager.h"

class TaskManagerSerializer {
    const QString kSaveFile = "taskmanager.dat";

public:
    TaskManagerSerializer() = default;

    void serialize(const TaskManager& taskManager);

    void deserialize(TaskManager& taskManager);

};

#endif // SERIALIZER_H
