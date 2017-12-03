#include "Task.h"

Task::Task(const QString& name,
           const QString& description,
           const QDateTime& startTime,
           Trigger::IntervalType internalType,
           qint64 interval,
           const QString& rawOperation,
           const QStringList& arguments)
    : name(name), description(description)
{
    Operation* operation = new Operation(rawOperation, arguments);
    operations.append(operation);
    // Used when debugging
    // TODO: use journal system catch the following two.
    connect(&operation->process, &QProcess::readyReadStandardOutput, [=]() {
        qDebug() << operation->process.readAllStandardOutput().data();
    });
    connect(&operation->process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [&](int code, QProcess::ExitStatus s) {
        qDebug() << code << " " << s;
    });
    connect(&operation->process, &QProcess::stateChanged, [=]() {
        qDebug() << operation->process.state();
        emit stateChanged();
    });

    Trigger* trigger = new Trigger(startTime, internalType, interval);
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
