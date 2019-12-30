
#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "KMLObject.h"

KMLObject::KMLObject(const QString &id)
    : mId(id)
{
}

KMLObject::~KMLObject()
{
}

QString KMLObject::id() const
{
    return mId;
}

void KMLObject::setId(const QString &_id)
{
    mId = _id;
}

QString KMLIcon::RefreshModeTypeToString(KMLIcon::RefreshModeType type)
{
    static QStringList types = {"OnChange", "OnInterval", "OnExpire"};

    return types.value((int) type, "OnChange");
}

QIcon KMLIcon::icon() const
{
    return QIcon(mIconPath);
}

void KMLIcon::setIcon(const QString &iconPath)
{
    mIconPath = iconPath;
}

KMLIcon::RefreshModeType KMLIcon::refreshMode() const
{
    return mRefreshMode;
}

void KMLIcon::setRefreshMode(const RefreshModeType &refreshMode)
{
    mRefreshMode = refreshMode;
}

int KMLIcon::refreshInterval() const
{
    return mRefreshInterval;
}

void KMLIcon::setRefreshInterval(int refreshInterval)
{
    mRefreshInterval = refreshInterval;
}

QDomElement KMLIcon::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("Icon");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    {
        QDomElement ele  = doc.createElement("href");
        QDomText    text = doc.createTextNode(mIconPath);
        ele.appendChild(text);
        element.appendChild(ele);
    }
    {

        QDomElement ele  = doc.createElement("refreshMode");
        QDomText    text = doc.createTextNode(RefreshModeTypeToString(mRefreshMode));
        ele.appendChild(text);
        element.appendChild(ele);
    }
    if (mRefreshMode == OnInterval)
    {
        QDomElement ele  = doc.createElement("refreshInterval");
        QDomText    text = doc.createTextNode(QString::number(mRefreshInterval));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    return element;
}

bool KMLIcon::fromDom(const QDomElement &dom)
{
    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {

        QDomNodeList hrefs = dom.elementsByTagName("href");
        if (!hrefs.isEmpty())
        {
            QString href = hrefs.item(0).toElement().text();
            setIcon(href);
        }

        QDomNodeList refreshModes = dom.elementsByTagName("refreshMode");
        if (!refreshModes.isEmpty())
        {
            QString refreshMode = refreshModes.item(0).toElement().text();
            bool    ok          = false;

            RefreshModeType value = (RefreshModeType) refreshMode.toInt(&ok);
            if (!ok)
                return false;

            setRefreshMode(value);
        }

        QDomNodeList refreshIntervals = dom.elementsByTagName("refreshInterval");
        if (!refreshIntervals.isEmpty())
        {
            QString refreshInterval = refreshIntervals.item(0).toElement().text();
            bool    ok              = false;

            int interval = refreshInterval.toInt(&ok);
            if (!ok)
                return false;

            setRefreshInterval(interval);
        }
    }
    return true;
}

KMLLocation::KMLLocation(double lng, double lat, double alt, const QString &id)
    : mLongitude(lng)
    , mLatitude(lat)
    , mAltitude(alt)
{
}

double KMLLocation::longitude() const
{
    return mLongitude;
}

void KMLLocation::setLongitude(double longitude)
{
    mLongitude = longitude;
}

double KMLLocation::latitude() const
{
    return mLatitude;
}

void KMLLocation::setLatitude(double latitude)
{
    mLatitude = latitude;
}

double KMLLocation::altitude() const
{
    return mAltitude;
}

void KMLLocation::setAltitude(double altitude)
{
    mAltitude = altitude;
}

QDomElement KMLLocation::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("Location");
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

    return element;
}

bool KMLLocation::fromDom(const QDomElement &dom)
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
    }
    return true;
}

QDomElement KMLCoordinates::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("coordinates");

    QStringList coordinates;
    for (int i = 0; i < count(); ++i)
    {
        coordinates << QString::asprintf(".10lf%,.10lf%,.10lf%", at(i).mLongitude, at(i).mLatitude, at(i).mAltitude);
    }
    QDomText text = doc.createTextNode(coordinates.join(" "));
    element.appendChild(text);

    return element;
}

bool KMLCoordinates::fromDom(const QDomElement &dom)
{
    QDomNodeList nodes = dom.childNodes();
    if (!nodes.isEmpty())
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            QDomElement ele = nodes.at(i).toElement();
            if (ele.tagName() == "coordinates")
            {
                QString str = ele.text().trimmed();

                QStringList    list = str.split(" ", QString::SkipEmptyParts);
                bool           ok   = false;
                KMLCoordinates tmp;
                tmp.reserve(list.size());
                for (const QString &coordinate : list)
                {
                    KMLCoordinate coord;
                    QStringList   values = coordinate.trimmed().split(",");

                    if (values.size() == 3)
                    {
                        coord.mAltitude = values.at(2).toDouble(&ok);
                        if (!ok)
                            return false;
                    }

                    if (values.size() >= 2)
                    {
                        coord.mLatitude = values.at(1).toDouble(&ok);
                        if (!ok)
                            return false;

                        coord.mLongitude = values.at(0).toDouble(&ok);
                        if (!ok)
                            return false;
                    }

                    if (values.size() < 2)
                        return false;
                    tmp.push_back(coord);
                }
                swap(tmp);
                return true;
            }
        }
    }
    return false;
}
