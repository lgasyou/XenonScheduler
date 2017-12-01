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
    QProcess* process = nullptr;

public:
    Operation(const QString& program,
              const QStringList& arguments = QStringList())
        : program(program),
          arguments(arguments)
    {}

    Operation() = default;

    void killProcess() {
        if (process) {
            process->kill();
            qDebug() << process->state();
            qDebug() << "Operation::killProcess()";
        }
    }

};

#endif // OPERATION_H
