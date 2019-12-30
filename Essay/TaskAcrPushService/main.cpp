#include "EventsWidget.h"

#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include <QApplication>
#include <QtGlobal>
#include <acr/common/VAcrGlobal.h>

using namespace vsr;
using namespace vsr::acr;

#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QString QT_MESSAGE_PATTERN =
        "[%{type} "
        "%{threadid} "
        "%{time yyyyMMdd hhmmss} "
        "%{function}] "
        "%{message}";

    qSetMessagePattern(QT_MESSAGE_PATTERN);
    QApplication a(argc, argv);
    VAcrLibrary  acrlib(argc, (const char **) argv);

    ModuleMeta meta;

    meta.name   = "时间服务";
    meta.vender = "VSurs";
    meta.series = "Test-TimeService";
    meta.serial = "XiuShanMobieSurvey";

    EventsWidget widget(meta, 1111);
    widget.show();
    widget.initializeModuleDiscovery();
    widget.asynchRunService();

    QList<EventsWidget *> widgets;
#if 0
    for (int i = 0; i < 2; ++i)
    {
        meta.name   = "测试服务" + std::to_string(i);
        meta.vender = "VSurs";
        meta.series = "TestService" + std::to_string(i);
        meta.serial = "XiuShanMobieSurvey";

        EventsWidget *widget0 = new EventsWidget(meta, 1112 + i);
        widget0->show();
        widget0->initializeModuleDiscovery();
        widget0->asynchRunService();
        widgets.push_back(widget0);
    }
#endif
    int ret = a.exec();

    qDeleteAll(widgets);
    widgets.clear();

    return ret;
}
