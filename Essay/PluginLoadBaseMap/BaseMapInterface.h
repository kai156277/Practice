#ifndef BASEMAPINTERFACE_H
#define BASEMAPINTERFACE_H

#include "BasePluginInterface.h"

#include <QPixmap>
// TODO: 使用 proj4 的 PROJ_COORD 代替
#include <QPointF>
#include <QString>
#include <QtPlugin>

class BaseMapInterface : public BasePluginInterface
{
public:
    virtual ~BaseMapInterface() = default;

    virtual bool loadFromFile(const QString &file) = 0;

    virtual QString filter() const  = 0;
    virtual QString suffix() const  = 0;
    virtual QPixmap baseMap() const = 0;
    virtual QPointF pos() const     = 0;
};

Q_DECLARE_INTERFACE(BaseMapInterface, "SuperSurs.GuideLine.IO.BaseMapInterface")

#endif   // BASEMAPINTERFACE_H
