
#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "KMLAbstractView.h"

KMLAbstractView::KMLAbstractView(KMLAbstractView::Type type, const QString &id)
    : KMLObject(id)
    , mType(type)
{
}

KMLAbstractView::Type KMLAbstractView::type() const
{
    return mType;
}

void KMLAbstractView::setType(const Type &type)
{
    mType = type;
}

KMLLookAt::KMLLookAt()
    : KMLAbstractView(LookAt)
{
}

KMLLookAt::KMLLookAt(double lng, double lat, double alt, double range, const QString &id)
    : KMLAbstractView(LookAt, id)
    , mLongitude(lng)
    , mLatitude(lat)
    , mAltitude(alt)
    , mRange(range)
{
}

double KMLLookAt::longitude() const
{
    return mLongitude;
}

void KMLLookAt::setLongitude(double longitude)
{
    mLongitude = longitude;
}

double KMLLookAt::latitude() const
{
    return mLatitude;
}

void KMLLookAt::setLatitude(double latitude)
{
    mLatitude = latitude;
}

double KMLLookAt::altitude() const
{
    return mAltitude;
}

void KMLLookAt::setAltitude(double altitude)
{
    mAltitude = altitude;
}

double KMLLookAt::range() const
{
    return mRange;
}

void KMLLookAt::setRange(double range)
{
    mRange = range;
}

QDomElement KMLLookAt::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("LookAt");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    {
        QDomElement ele  = doc.createElement("longitude");
        QDomText    text = doc.createTextNode(QString::number(mLongitude, 'f', 10));
        ele.appendChild(text);
        element.appendChild(ele);
    }
    {

        QDomElement ele  = doc.createElement("latitude");
        QDomText    text = doc.createTextNode(QString::number(mLatitude, 'f', 10));
        ele.appendChild(text);
        element.appendChild(ele);
    }
    {
        QDomElement ele  = doc.createElement("altitude");
        QDomText    text = doc.createTextNode(QString::number(mAltitude, 'f', 10));
        ele.appendChild(text);
        element.appendChild(ele);
    }
    {
        QDomElement ele  = doc.createElement("range");
        QDomText    text = doc.createTextNode(QString::number(mRange, 'f', 10));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    return element;
}

bool KMLLookAt::fromDom(const QDomElement &dom)
{

    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {
        {
            QDomNodeList nodes = dom.elementsByTagName("longitude");
            if (!nodes.isEmpty())
            {
                QString str = nodes.item(0).toElement().text();
                bool    ok  = false;

                double value = str.toDouble(&ok);
                if (!ok)
                    return false;

                setLongitude(value);
            }
        }

        {
            QDomNodeList nodes = dom.elementsByTagName("latitude");
            if (!nodes.isEmpty())
            {
                QString str = nodes.item(0).toElement().text();
                bool    ok  = false;

                double value = str.toDouble(&ok);
                if (!ok)
                    return false;

                setLatitude(value);
            }
        }

        {
            QDomNodeList nodes = dom.elementsByTagName("altitude");
            if (!nodes.isEmpty())
            {
                QString str = nodes.item(0).toElement().text();
                bool    ok  = false;

                double value = str.toDouble(&ok);
                if (!ok)
                    return false;

                setAltitude(value);
            }
        }

        {
            QDomNodeList nodes = dom.elementsByTagName("range");
            if (!nodes.isEmpty())
            {
                QString str = nodes.item(0).toElement().text();
                bool    ok  = false;

                double value = str.toDouble(&ok);
                if (!ok)
                    return false;

                setRange(value);
            }
        }
    }
    return true;
}
