#include "Task.h"

static QString ProcessExitStatus2StringHelper(int code, QProcess::ExitStatus status);

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
    addOperation(operation);

    Trigger* trigger = new Trigger(startTime, internalType, interval);
    addTrigger(trigger);
}

const QVector<Operation*>& Task::getOperations() const {
    return operations;
}

const QVector<Trigger*>& Task::getTriggers() const {
    return triggers;
}

const QString& Task::getLastRunResult() const {
    return lastRunResult;
}

const QDateTime& Task::getLastStartTime() const {
    return lastStartTime;
}

const QString& Task::getDescription() const {
    return description;
}

void Task::setDescription(const QString& value) {
    description = value;
}

void Task::addOperation(Operation *operation) {
    operations.append(operation);
    // Used when debugging
    // TODO: use journal system catch the following two.
    connect(&operation->process, &QProcess::readyReadStandardOutput, [=]() {
        qDebug() << operation->process.readAllStandardOutput().data();
    });
    connect(&operation->process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int c, QProcess::ExitStatus s) {
        lastRunResult = ::ProcessExitStatus2StringHelper(c, s);
        emit stateChanged();
    });
    connect(&operation->process, &QProcess::stateChanged, [=]() {
        if (operation->process.state() != QProcess::NotRunning) {
            emit stateChanged();
        }
    });
}

const QString& Task::getName() const {
    return name;
}

void Task::setName(const QString& value) {
    name = value;
}

static QString ProcessExitStatus2StringHelper(int code, QProcess::ExitStatus status) {
    return (status == 0 ? "Normal Exit" : "Crashed Exit") + QString(" (%1)").arg(code);
}
