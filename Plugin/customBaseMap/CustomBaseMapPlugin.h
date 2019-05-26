#ifndef CUSTOMBASEMAPPLUGIN_H
#define CUSTOMBASEMAPPLUGIN_H

#include <BaseMapInterface.h>
#include <QPointF>

class CustomBaseMapPlugin
    : public QObject
    , public BaseMapInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "SuperSurs.GuideLine.IO.CustomBaseMapPlugin" FILE "customBaseMap.json")
    Q_INTERFACES(BaseMapInterface)
public:
    CustomBaseMapPlugin();

    virtual bool loadFromFile(const QString &file) override;

    virtual QString filter() const override;
    virtual QString suffix() const override;
    virtual QPixmap baseMap() const override;
    virtual QPointF pos() const override;
    virtual QString name() const override;
    virtual QString description() const override;

private:
    QPointF mPos;
    QPixmap mPixmap;
};

#endif   // CUSTOMBASEMAPPLUGIN_H
