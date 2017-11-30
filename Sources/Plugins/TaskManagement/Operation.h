#ifndef OPERATION_H
#define OPERATION_H

#include <QString>
#include <QStringList>

class Operation {
public:
    QString program;
    QStringList arguments;

    Operation(const QString& program,
              const QStringList& arguments = QStringList())
        : program(program),
          arguments(arguments)
    {}

    Operation() = default;

};

#endif // OPERATION_H
