#ifndef TRIGGER_H
#define TRIGGER_H

#include <QDateTime>

class Trigger {
public:
    enum IntervalType : qint64 {
        Day,
        Week,
        Month,
        Once,
        StartingUp,
        Login,
    };

    QDateTime nextStartTime;
    IntervalType intervalType;
    qint64 interval;

public:
    Trigger(const QDateTime& nextStartTime, IntervalType intervalType, qint64 interval)
        : nextStartTime(nextStartTime),
          intervalType(intervalType),
          interval(interval)
    {}

    Trigger(const Trigger& rhs) = delete;
    ~Trigger() = default;

    bool triggered() const {
        return isPeriodic() && nextStartTime <= QDateTime::currentDateTime();
    }

    bool isPeriodic() const {
        return intervalType != Once && intervalType != StartingUp && intervalType != Login;
    }

    void updateNextStartTime() {
        switch (intervalType) {
        case Day:
            nextStartTime = nextStartTime.addDays(interval);
            break;
        case Week:
            nextStartTime = nextStartTime.addDays(7*interval);
            break;
        case Month:
            nextStartTime = nextStartTime.addMonths(interval);
            break;
        default:
            break;
        }
    }

};

#endif // TRIGGER_H
