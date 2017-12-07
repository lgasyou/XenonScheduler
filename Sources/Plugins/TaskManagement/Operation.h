#ifndef OPERATION_H
#define OPERATION_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

class Task;

class Operation {
    friend QDataStream& operator<<(QDataStream&, const Operation&);
    friend QDataStream& operator>>(QDataStream&, Operation&);

public:
    QString program;
    QStringList arguments;
    QProcess process;

public:
    Operation(const QString& program,
              const QStringList& arguments = QStringList())
        : program(program),
          arguments(arguments)
    {}

    Operation() = default;
    Operation(const Operation& rhs) = delete;
    ~Operation() = default;

    void killActiveProcess() {
        process.kill();
    }

};

#endif // OPERATION_H
