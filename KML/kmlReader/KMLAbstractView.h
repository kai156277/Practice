#pragma once   // KMLABSTRACTVIEW_H

#include "KMLObject.h"

class KMLAbstractView : public KMLObject
{
public:
    enum Type
    {
        LookAt = 0,
    };

    KMLAbstractView(Type type, const QString &id = QString());

    Type type() const;
    void setType(const Type &type);

private:
    Type mType;
};

class KMLLookAt : public KMLAbstractView
{
public:
    KMLLookAt();
    KMLLookAt(double lng, double lat, double alt = 0, double range = 0, const QString &id = QString());

    double longitude() const;
    void   setLongitude(double longitude);

    double latitude() const;
    void   setLatitude(double latitude);

    double altitude() const;
    void   setAltitude(double altitude);

    double range() const;
    void   setRange(double range);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    double mLongitude;
    double mLatitude;
    double mAltitude;
    double mRange;
};
