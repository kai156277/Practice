#include "Demo.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QString QT_MESSAGE_PATTERN =
        "[%{type} "
        "%{threadid} "
        "%{time yyyyMMdd hhmmss }"
        "%{function}] "
        "%{message}";

    qSetMessagePattern(QT_MESSAGE_PATTERN);
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("ControlDemo");
    QCoreApplication::setOrganizationName("XiuShan");
    QCoreApplication::setOrganizationDomain("supersurs.com");
    Demo w;
    w.show();
    qDebug() << "main thread";

    return a.exec();
}
