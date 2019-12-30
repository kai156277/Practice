#include "MainWindow.h"

#include <QApplication>
#include <QtPlugin>

//Q_IMPORT_PLUGIN(CustomBaseMapPlugin)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();

    qDebug() << QImageReader::supportedImageFormats();
    return a.exec();
}
