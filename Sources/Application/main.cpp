#include "Scheduler.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Scheduler scheduler;
    scheduler.show();

    return a.exec();
}
