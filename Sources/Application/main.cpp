#include "Scheduler.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Scheduler w;
    w.show();

    return a.exec();
}
