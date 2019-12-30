#pragma once   // KMLGEOMETRY_H

#include "KMLObject.h"

#include <functional>

class KMLGeometry : public KMLObject
{
public:
    enum Type : int
    {
        Invalid = -1,
        Point   = 0,
        LineString,
        LinearRing,
        Polygon,
        MultiGeometry
    };
    KMLGeometry(Type type, const QString &id = QString());

    Type type() const;
    void setType(const Type &type);

    static QString typeToString(Type type);
    static Type    typeFromString(const QString &type);

private:
    Type mType;

    static QStringList typeStrings;
};

class KMLPoint : public KMLGeometry
{
public:
    KMLPoint(const KMLCoordinate &coord = KMLCoordinate(), const QString &id = QString());

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

    KMLCoordinate coord() const;
    void          setCoord(const KMLCoordinate &coord);

private:
    KMLCoordinate mCoord;
};

class KMLLineString : public KMLGeometry
{
public:
    KMLLineString(const KMLCoordinates &coords = KMLCoordinates(), const QString &id = QString());

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

    KMLCoordinates coords() const;
    void           setCoords(const KMLCoordinates &coords);

private:
    KMLCoordinates mCoords;
};

class KMLLinearRing : public KMLGeometry
{
public:
    KMLLinearRing(const KMLCoordinates &coords = KMLCoordinates(), const QString &id = QString());
    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

    KMLCoordinates coords() const;
    void           setCoords(const KMLCoordinates &coords);

private:
    KMLCoordinates mCoords;
};

class KMLPolygon : public KMLGeometry
{
public:
    KMLPolygon(const KMLLinearRing &         outer = KMLLinearRing(),
               const QVector<KMLLinearRing> &inner = QVector<KMLLinearRing>(),
               const QString &               id    = QString());

    KMLLinearRing outerBoundary() const;
    void          setOuterBoundary(const KMLLinearRing &outer);

    QVector<KMLLinearRing> innerBoundary() const;
    void                   setInnerBoundary(const QVector<KMLLinearRing> &inner);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    KMLLinearRing          mOuterBoundary;
    QVector<KMLLinearRing> mInnerBoundary;
};

class KMLMultiGeometry : public KMLGeometry
{
public:
    KMLMultiGeometry(const QString &id = QString());

    KMLGeometry *at(int index);

    int  count() const;
    void addGeometry(KMLGeometry *geometry);
    void removeGeometry(int index);
    void deleteAllGeometry();

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    QVector<KMLGeometry *> mGeometrys;
};
