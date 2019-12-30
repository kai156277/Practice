#include "AnalogClock.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnalogClock  w;
    w.show();

    return a.exec();
}
