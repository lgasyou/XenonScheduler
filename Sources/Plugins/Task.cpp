#include "Task.h"

Task::Task(const QString& name,
           const QString& description,
           const QDateTime& startTime,
           Trigger::Internal internal,
           const QString& rawOperation,
           const QStringList& arguments)
    : name(name), description(description)
{
    Operation operation(rawOperation, arguments);
    Trigger trigger(startTime, internal);
    operations.append(operation);
    triggers.append(trigger);
}

QString Task::getLastRunResult() const {
    return lastRunResult;
}

void Task::setLastRunResult(const QString& value) {
    lastRunResult = value;
}

QDateTime Task::getLastStartTime() const {
    return lastStartTime;
}

void Task::setLastStartTime(const QDateTime& value) {
    lastStartTime = value;
}

QString Task::getDescription() const {
    return description;
}

void Task::setDescription(const QString& value) {
    description = value;
}

QString Task::getName() const {
    return name;
}

void Task::setName(const QString& value) {
    name = value;
}
