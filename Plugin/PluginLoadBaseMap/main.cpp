#include "MainWindow.h"

#include <QApplication>
#include <QtPlugin>

//Q_IMPORT_PLUGIN(CustomBaseMapPlugin)

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("LoadBaseMap");
    QCoreApplication::setOrganizationName("XiuShan");
    QCoreApplication::setOrganizationDomain("supersurs.com");
    MainWindow w;
    w.show();

    qDebug() << QImageReader::supportedImageFormats();
    return a.exec();
}
