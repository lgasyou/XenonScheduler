#ifndef TRIGGER_H
#define TRIGGER_H

#include <QDateTime>

class Trigger {
public:
    enum Internal : qint64 {
        Day         = 600*24,
        Week        = Day*7,
        Month       = Day*30,
        Once        = -1,
        StartingUp  = -2,
        login       = -3,
    };

    Internal internal;
    QDateTime nextStartTime;

    Trigger(const QDateTime& nextStartTime, Internal internal)
        : nextStartTime(nextStartTime),
          internal(internal)
    {}

    Trigger() = default;
};

#endif // TRIGGER_H
