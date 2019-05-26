#include "CustomBaseMapPlugin.h"

#include <QtCore>

#pragma execution_character_set("utf-8")

CustomBaseMapPlugin::CustomBaseMapPlugin()
{
    QSettings config;
    config.setValue("TestValue", 100);
    mPos = QPointF(0, 0);
}

bool CustomBaseMapPlugin::loadFromFile(const QString &file)
{
    mPos.setX(QTime::currentTime().minute());
    mPos.setY(QTime::currentTime().second());
    return mPixmap.load(file, "PNG");
}

QString CustomBaseMapPlugin::filter() const
{
    return "PNG 图像(*.png )";
}

QString CustomBaseMapPlugin::suffix() const
{
    return "png";
}

QPixmap CustomBaseMapPlugin::baseMap() const
{
    return mPixmap;
}

QPointF CustomBaseMapPlugin::pos() const
{
    return mPos;
}

QString CustomBaseMapPlugin::name() const
{
    return "PNG文件插件";
}

QString CustomBaseMapPlugin::description() const
{
    return "这是用来加载PNG文件的插件";
}
