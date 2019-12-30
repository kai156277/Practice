#include "RequestModule.h"
#include "RequestWidget.h"

#include <QApplication>
#include <QString>
#include <acr/common/VAcrGlobal.h>

using namespace vsr;
using namespace vsr::acr;

int main(int argc, char *argv[])
{
    QApplication  a(argc, argv);
    VAcrLibrary   acrlib(argc, (const char **) argv);
    RequestWidget w;
    w.show();

    return a.exec();
}
