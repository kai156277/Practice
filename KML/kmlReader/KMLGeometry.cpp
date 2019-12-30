
#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "KMLGeometry.h"

QStringList KMLGeometry::typeStrings = {
    "Point",
    "LineString",
    "LinearRing",
    "Polygon",
    "MultiGeometry"};
KMLGeometry::KMLGeometry(KMLGeometry::Type type, const QString &id)
    : KMLObject(id)
    , mType(type)
{
}

KMLGeometry::Type KMLGeometry::type() const
{
    return mType;
}

void KMLGeometry::setType(const Type &type)
{
    mType = type;
}

QString KMLGeometry::typeToString(KMLGeometry::Type type)
{
    return typeStrings.value((int) type);
}

KMLGeometry::Type KMLGeometry::typeFromString(const QString &type)
{
    return (KMLGeometry::Type) typeStrings.indexOf(type);
}

KMLPoint::KMLPoint(const KMLCoordinate &coords, const QString &id)
    : KMLGeometry(Point, id)
    , mCoord(coords)
{
}

QDomElement KMLPoint::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("Point");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    {
        QDomElement element = doc.createElement("coordinates");
        QString     tmp     = QString::asprintf(".10lf%,.10lf%,.10lf%",
                                        mCoord.mLongitude,
                                        mCoord.mLatitude,
                                        mCoord.mAltitude);
        QDomText    text    = doc.createTextNode(tmp);
        element.appendChild(text);
    }

    {
        QDomElement ele  = doc.createElement("extrude");
        QDomText    text = doc.createTextNode(QString::number(0));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    return element;
}

bool KMLPoint::fromDom(const QDomElement &dom)
{
    if (dom.tagName() != typeToString(Point))
    {
        return false;
    }

    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    QDomNodeList nodes = dom.childNodes();
    if (!nodes.isEmpty())
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            QDomElement ele = nodes.at(i).toElement();
            if (ele.tagName() == "coordinates")
            {
                QString     str    = ele.text().trimmed();
                bool        ok     = false;
                QStringList values = str.split(",");

                if (values.size() == 3)
                {
                    mCoord.mAltitude = values.at(2).toDouble(&ok);
                    if (!ok)
                        return false;
                }

                if (values.size() >= 2)
                {
                    mCoord.mLatitude = values.at(1).toDouble(&ok);
                    if (!ok)
                        return false;

                    mCoord.mLongitude = values.at(0).toDouble(&ok);
                    if (!ok)
                        return false;
                }

                if (values.size() < 2)
                    return false;

                return true;
            }
        }
    }
    return false;
}

KMLCoordinate KMLPoint::coord() const
{
    return mCoord;
}

void KMLPoint::setCoord(const KMLCoordinate &coord)
{
    mCoord = coord;
}

KMLLineString::KMLLineString(const KMLCoordinates &coords, const QString &id)
    : KMLGeometry(LineString, id)
    , mCoords(coords)
{
}

QDomElement KMLLineString::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("LineString");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    element.appendChild(mCoords.toDom());

    {
        QDomElement ele  = doc.createElement("extrude");
        QDomText    text = doc.createTextNode(QString::number(0));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    {
        QDomElement ele  = doc.createElement("tessellate");
        QDomText    text = doc.createTextNode(QString::number(0));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    return element;
}

bool KMLLineString::fromDom(const QDomElement &dom)
{
    if (dom.tagName() != typeToString(LineString))
    {
        return false;
    }

    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {
        mCoords.fromDom(dom);
    }
    return false;
}

KMLCoordinates KMLLineString::coords() const
{
    return mCoords;
}

void KMLLineString::setCoords(const KMLCoordinates &coords)
{
    mCoords = coords;
}

KMLLinearRing::KMLLinearRing(const KMLCoordinates &coords, const QString &id)
    : KMLGeometry(LinearRing, id)
    , mCoords(coords)
{
}

QDomElement KMLLinearRing::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("LinearRing");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    element.appendChild(mCoords.toDom());

    {
        QDomElement ele  = doc.createElement("extrude");
        QDomText    text = doc.createTextNode(QString::number(0));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    {
        QDomElement ele  = doc.createElement("tessellate");
        QDomText    text = doc.createTextNode(QString::number(0));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    return element;
}

bool KMLLinearRing::fromDom(const QDomElement &dom)
{
    if (dom.tagName() != typeToString(LinearRing))
    {
        return false;
    }

    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {
        mCoords.fromDom(dom);
    }
    return false;
}

KMLCoordinates KMLLinearRing::coords() const
{
    return mCoords;
}

void KMLLinearRing::setCoords(const KMLCoordinates &coords)
{
    mCoords = coords;
}

KMLPolygon::KMLPolygon(const KMLLinearRing &outer, const QVector<KMLLinearRing> &inner, const QString &id)
    : KMLGeometry(Polygon, id)
    , mOuterBoundary(outer)
    , mInnerBoundary(inner)
{
}

QDomElement KMLPolygon::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("Polygon");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    {
        QDomElement outer = doc.createElement("outerBoundaryIs");
        outer.appendChild(mOuterBoundary.toDom());
        element.appendChild(outer);
    }

    for (int i = 0; i < mInnerBoundary.size(); ++i)
    {
        QDomElement inner = doc.createElement("innerBoundaryIs");
        inner.appendChild(mInnerBoundary.at(i).toDom());
        element.appendChild(inner);
    }

    {
        QDomElement ele  = doc.createElement("extrude");
        QDomText    text = doc.createTextNode(QString::number(0));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    {
        QDomElement ele  = doc.createElement("tessellate");
        QDomText    text = doc.createTextNode(QString::number(0));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    return element;
}

bool KMLPolygon::fromDom(const QDomElement &dom)
{
    if (dom.tagName() != typeToString(Polygon))
    {
        return false;
    }

    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {
        QDomNodeList outers = dom.elementsByTagName("outerBoundaryIs");
        if (!outers.isEmpty())
        {
            QDomElement ele = outers.at(0).firstChildElement(typeToString(LinearRing));
            if (!mOuterBoundary.fromDom(ele))
                return false;
        }
        else
        {
            return false;
        }

        QVector<KMLLinearRing> tmp;

        QDomNodeList inners = dom.elementsByTagName("innerBoundaryIs");
        int          size   = inners.size();
        tmp.reserve(size);
        for (int i = 0; i < size; ++i)
        {
            KMLLinearRing inner;
            QDomElement   ele = inners.at(i).firstChildElement(typeToString(LinearRing));
            inner.fromDom(ele);
            tmp.push_back(inner);
        }
        mInnerBoundary.swap(tmp);
    }
    return true;
}

KMLLinearRing KMLPolygon::outerBoundary() const
{
    return mOuterBoundary;
}

void KMLPolygon::setOuterBoundary(const KMLLinearRing &outer)
{
    mOuterBoundary = outer;
}

QVector<KMLLinearRing> KMLPolygon::innerBoundary() const
{
    return mInnerBoundary;
}

void KMLPolygon::setInnerBoundary(const QVector<KMLLinearRing> &inner)
{
    mInnerBoundary = inner;
}

KMLMultiGeometry::KMLMultiGeometry(const QString &id)
    : KMLGeometry(MultiGeometry, id)
{
}

KMLGeometry *KMLMultiGeometry::at(int index)
{
    return mGeometrys.value(index, nullptr);
}

int KMLMultiGeometry::count() const
{
    return mGeometrys.count();
}

void KMLMultiGeometry::addGeometry(KMLGeometry *geometry)
{
    mGeometrys.push_back(geometry);
}

void KMLMultiGeometry::removeGeometry(int index)
{
    if (0 <= index && index < mGeometrys.size())
        mGeometrys.removeAt(index);
}

void KMLMultiGeometry::deleteAllGeometry()
{
    qDeleteAll(mGeometrys);
    mGeometrys.clear();
}

QDomElement KMLMultiGeometry::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("MultiGeometry");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    for (int i = 0; i < mGeometrys.size(); ++i)
    {
        element.appendChild(mGeometrys.at(i)->toDom());
    }

    return element;
}

bool KMLMultiGeometry::fromDom(const QDomElement &dom)
{
    if (dom.tagName() != typeToString(MultiGeometry))
    {
        return false;
    }

    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {
        deleteAllGeometry();
        QDomNodeList styles = dom.childNodes();
        if (!styles.isEmpty())
        {
            for (int i = 0; i < styles.size(); ++i)
            {
                QDomElement       ele   = styles.item(i).toElement();
                KMLGeometry::Type t     = KMLGeometry::typeFromString(ele.tagName());
                KMLGeometry *     style = nullptr;
                switch (t)
                {
                case Point:
                {
                    style = new KMLPoint();
                    break;
                }
                case LineString:
                {
                    style = new KMLLineString();
                    break;
                }
                case LinearRing:
                {
                    style = new KMLLinearRing();
                    break;
                }
                case Polygon:
                {
                    style = new KMLPolygon();
                    break;
                }
                default:
                    break;
                }
                style->fromDom(ele);
                mGeometrys.push_back(style);
            }
        }
    }
    return true;
}
