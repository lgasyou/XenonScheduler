#include "TaskManagerSerializer.h"
#include <QDataStream>
#include <QDebug>
#include <utility>

// TaskManager Serialization
QDataStream& operator<<(QDataStream& out, const TaskManager& taskManager);
QDataStream& operator>>(QDataStream& in, TaskManager& taskManager);

// Task Serialization
QDataStream& operator<<(QDataStream& out, const Task& task);
QDataStream& operator>>(QDataStream& in, Task& task);

// Trigger Serialization
QDataStream& operator<<(QDataStream& out, const Trigger& trigger);
QDataStream& operator>>(QDataStream& in, Trigger& trigger);

// Operation Serialization
QDataStream& operator<<(QDataStream& out, const Operation& operation);
QDataStream& operator>>(QDataStream& in, Operation& operation);

void TaskManagerSerializer::serialize(const TaskManager &taskManager) {
    QFile file(kSaveFile);
    if (file.open(QFile::WriteOnly)) {
        QDataStream out(&file);
        out << taskManager;
        file.close();
    }
}

void TaskManagerSerializer::deserialize(TaskManager &taskManager) {
    QFile file(kSaveFile);
    if (file.open(QFile::ReadOnly)) {
        QDataStream in(&file);
        in >> taskManager;
        file.close();
    }
}

// TaskManager Serialization
QDataStream& operator<<(QDataStream& out, const TaskManager& taskManager) {
    out << taskManager.checkInterval;
    out << taskManager.tasks.size();
    for (const Task* task : taskManager.tasks) {
        out << *task;
    }
    return out;
}

QDataStream& operator>>(QDataStream& in, TaskManager& taskManager) {
    in >> taskManager.checkInterval;
    int taskCnt = 0;
    in >> taskCnt;
    for (int i = 0; i != taskCnt; ++i) {
        Task* task = new Task();
        in >> *task;
        taskManager.append(task);
    }
    return in;
}

// Task Serialization
QDataStream& operator<<(QDataStream& out, const Task& task) {
    out << task.name << task.description << task.lastStartTime << task.lastRunResult;
    out << task.triggers.size();
    for (const Trigger* t : task.triggers) {
        out << *t;
    }
    out << task.operations.size();
    for (const Operation* o : task.operations) {
        out << *o;
    }
    return out;
}

QDataStream& operator>>(QDataStream& in, Task& task) {
    in >> task.name >> task.description >> task.lastStartTime >> task.lastRunResult;
    int triggerCnt = 0;
    in >> triggerCnt;
    for (int i = 0; i != triggerCnt; ++i) {
        Trigger* trigger = new Trigger();
        in >> *trigger;
        task.addTrigger(trigger);
    }

    int operationCnt = 0;
    in >> operationCnt;
    for (int i = 0; i != operationCnt; ++i) {
        Operation* operation = new Operation();
        in >> *operation;
        task.addOperation(operation);
    }
    return in;
}

// Trigger Serialization
QDataStream& operator<<(QDataStream& out, const Trigger& trigger) {
    return out << trigger.nextStartTime << trigger.intervalType << trigger.interval;
}

QDataStream& operator>>(QDataStream& in, Trigger& trigger) {
    qint32 intervalType = 0;
    in >> trigger.nextStartTime >> intervalType >> trigger.interval;
    trigger.intervalType = static_cast<Trigger::IntervalType>(intervalType);
    return in;
}

// Operation Serialization
QDataStream& operator<<(QDataStream& out, const Operation& operation) {
    return out << operation.program << operation.arguments;
}

QDataStream& operator>>(QDataStream& in, Operation& operation) {
    return in >> operation.program >> operation.arguments;
}
