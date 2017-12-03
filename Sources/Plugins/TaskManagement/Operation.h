#ifndef OPERATION_H
#define OPERATION_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

class Operation {
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

    Operation(const Operation& rhs) = delete;
    ~Operation() = default;

    void killActiveProcess() {
        process.kill();
        qDebug() << process.state();
        qDebug() << "Operation::killProcess()";
    }

};

#endif // OPERATION_H
