
#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "KMLAbstractView.h"
#include "KMLFeature.h"
#include "KMLGeometry.h"
#include "KMLStyleSelector.h"

#define TagName_Name "name"
#define TagName_Visibility "visibility"
#define TagName_Open "open"
#define TagName_Address "address"
#define TagName_Description "description"

#define TagName_Placemark "Placemark"
#define TagName_Document "Document"
#define TagName_Folder "Folder"

KMLFeature::KMLFeature(const QString &id)
    : KMLObject(id)
    , mVisibility(true)
    , mOpen(true)
{
}

QString KMLFeature::name() const
{
    return mName;
}

void KMLFeature::setName(const QString &name)
{
    mName = name;
}

QDomElement KMLFeature::nameToDom() const
{
    if (mName.isEmpty())
        return QDomElement();

    QDomDocument doc;
    QDomElement  element = doc.createElement(TagName_Name);
    QDomText     text    = doc.createTextNode(mName);
    element.appendChild(text);
    return element;
}

bool KMLFeature::nameFromDom(const QDomElement &dom)
{
    if (dom.tagName() == TagName_Name)
    {
        setName(dom.text());
        return true;
    }
    else
    {
        return false;
    }
}

bool KMLFeature::visibility() const
{
    return mVisibility;
}

void KMLFeature::setVisibility(bool visibility)
{
    mVisibility = visibility;
}

QDomElement KMLFeature::visibilityToDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement(TagName_Visibility);
    QDomText     text    = doc.createTextNode(QString::number(mVisibility ? 1 : 0));
    element.appendChild(text);
    return element;
}

bool KMLFeature::visibilityFromDom(const QDomElement &dom)
{
    if (dom.tagName() == TagName_Visibility)
    {
        QString str = dom.text();
        bool    ok  = false;
        int     i   = str.toInt(&ok);
        if (!ok)
            return false;

        setVisibility((i == 1));
        return true;
    }
    else
    {
        return false;
    }
}

bool KMLFeature::open() const
{
    return mOpen;
}

void KMLFeature::setOpen(bool open)
{
    mOpen = open;
}

QDomElement KMLFeature::openToDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement(TagName_Open);
    QDomText     text    = doc.createTextNode(QString::number(mOpen ? 1 : 0));
    element.appendChild(text);
    return element;
}

bool KMLFeature::openFromDom(const QDomElement &dom)
{
    if (dom.tagName() == TagName_Open)
    {
        QString str = dom.text();
        bool    ok  = false;
        int     i   = str.toInt(&ok);
        if (!ok)
            return false;

        setOpen((i == 1));
        return true;
    }
    else
    {
        return false;
    }
}

QString KMLFeature::address() const
{
    return mAddress;
}

void KMLFeature::setAddress(const QString &address)
{
    mAddress = address;
}

QDomElement KMLFeature::addressToDom() const
{
    if (mAddress.isEmpty())
        return QDomElement();

    QDomDocument doc;
    QDomElement  element = doc.createElement(TagName_Address);
    QDomText     text    = doc.createTextNode(mAddress);
    element.appendChild(text);
    return element;
}

bool KMLFeature::addressFromDom(const QDomElement &dom)
{
    if (dom.tagName() == TagName_Address)
    {
        setName(dom.text());
        return true;
    }
    else
    {
        return false;
    }
}

QString KMLFeature::description() const
{
    return mDescription;
}

void KMLFeature::setDescription(const QString &description)
{
    mDescription = description;
}

QDomElement KMLFeature::descriptionToDom() const
{
    if (mDescription.isEmpty())
        return QDomElement();

    QDomDocument doc;
    QDomElement  element = doc.createElement(TagName_Description);
    QDomText     text    = doc.createTextNode(mDescription);
    element.appendChild(text);
    return element;
}

bool KMLFeature::descriptionFromDom(const QDomElement &dom)
{
    if (dom.tagName() == TagName_Description)
    {
        setName(dom.text());
        return true;
    }
    else
    {
        return false;
    }
}

KMLAbstractView *KMLFeature::abstractView() const
{
    return mAbstractView;
}

void KMLFeature::setAbstractView(KMLAbstractView *abstractView)
{
    mAbstractView = abstractView;
}

QDomElement KMLFeature::abstractViewToDom() const
{
    if (mAbstractView)
        return mAbstractView->toDom();
    else
        return QDomElement();
}

bool KMLFeature::abstractViewFromDom(const QDomElement &dom)
{
    if (mAbstractView)
        return mAbstractView->fromDom(dom);
    else
        return false;
}

#ifdef UNUSED
KMLStyleUrl KMLFeature::styleUrl() const
{
    return mStyleUrl;
}

void KMLFeature::setStyleUrl(const KMLStyleUrl &styleUrl)
{
    mStyleUrl = styleUrl;
}

QDomElement KMLFeature::styleUrlToDom() const
{
    return mStyleUrl.toDom();
}

bool KMLFeature::styleUrlFromDom(const QDomElement &dom)
{
    return mStyleUrl.fromDom(dom);
}
#endif

KMLStyle KMLFeature::style() const
{
    return mStyle;
}

void KMLFeature::setStyle(const KMLStyle &style)
{
    mStyle = style;
}

QDomElement KMLFeature::styleToDom() const
{
    return mStyle.toDom();
}

bool KMLFeature::styleFromDom(const QDomElement &dom)
{
    return mStyle.fromDom(dom);
}

KMLPlacemark::KMLPlacemark(const QString &id)
    : KMLFeature(id)
{
}

KMLGeometry *KMLPlacemark::geometry() const
{
    return mGeometry;
}

void KMLPlacemark::setGeometry(KMLGeometry *geometry)
{
    mGeometry = geometry;
}

QDomElement KMLPlacemark::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement(TagName_Placemark);
    element.appendChild(nameToDom());
    element.appendChild(visibilityToDom());
    element.appendChild(openToDom());
    element.appendChild(addressToDom());
    element.appendChild(descriptionToDom());
    element.appendChild(abstractViewToDom());
#ifdef UNUSED
    element.appendChild(styleUrlToDom());
#endif
    element.appendChild(styleToDom());

    if (mGeometry)
        element.appendChild(mGeometry->toDom());

    return element;
}

bool KMLPlacemark::fromDom(const QDomElement &dom)
{
    QDomNodeList nodes = dom.childNodes();
    if (!nodes.isEmpty())
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            QDomElement ele = nodes.at(i).toElement();
            if (ele.tagName() == TagName_Name)
                nameFromDom(ele);
            else if (ele.tagName() == TagName_Visibility)
                visibilityFromDom(ele);
            else if (ele.tagName() == TagName_Open)
                openFromDom(ele);
            else if (ele.tagName() == TagName_Address)
                addressFromDom(ele);
            else if (ele.tagName() == TagName_Description)
                descriptionFromDom(ele);
            else if (ele.tagName() == "Style")
                styleFromDom(ele);
            else
            {
                KMLGeometry::Type type = KMLGeometry::typeFromString(ele.tagName());
                switch (type)
                {
                case KMLGeometry::Point:
                {
                    if (mGeometry)
                        delete mGeometry;
                    mGeometry = new KMLPoint;
                    mGeometry->fromDom(ele);
                    break;
                }
                case KMLGeometry::LineString:
                {
                    if (mGeometry)
                        delete mGeometry;
                    mGeometry = new KMLLineString;
                    mGeometry->fromDom(ele);
                    break;
                }
                case KMLGeometry::LinearRing:
                {
                    if (mGeometry)
                        delete mGeometry;
                    mGeometry = new KMLLinearRing;
                    mGeometry->fromDom(ele);
                    break;
                }
                case KMLGeometry::Polygon:
                {
                    if (mGeometry)
                        delete mGeometry;
                    mGeometry = new KMLPolygon;
                    mGeometry->fromDom(ele);
                    break;
                }
                case KMLGeometry::MultiGeometry:
                {
                    if (mGeometry)
                        delete mGeometry;
                    mGeometry = new KMLMultiGeometry;
                    mGeometry->fromDom(ele);
                    break;
                }
                default:
                    break;
                }
            }
        }
#ifdef UNUSED
        styleUrlFromDom(dom);
        abstractViewFromDom(dom);
#endif
    }
    return true;
}
KMLDocument::KMLDocument(const QString &id)
    : KMLFeature(id)
{
}

QVector<KMLFeature *> KMLDocument::features() const
{
    return mFeatures;
}

void KMLDocument::setFeatures(const QVector<KMLFeature *> &features)
{
    mFeatures = features;
}

void KMLDocument::deleteAll()
{
    qDeleteAll(mFeatures);
    mFeatures.clear();
}

QDomElement KMLDocument::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement(TagName_Document);
    element.appendChild(nameToDom());
    element.appendChild(visibilityToDom());
    element.appendChild(openToDom());
    element.appendChild(addressToDom());
    element.appendChild(descriptionToDom());
    element.appendChild(abstractViewToDom());
#ifdef UNUSED
    element.appendChild(styleUrlToDom());
#endif
    element.appendChild(styleToDom());

    for (int i = 0; i < mFeatures.size(); ++i)
    {
        if (mFeatures.at(i))
            element.appendChild(mFeatures.at(i)->toDom());
    }

    return element;
}

bool KMLDocument::fromDom(const QDomElement &dom)
{
    QDomNodeList nodes = dom.childNodes();
    if (!nodes.isEmpty())
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            QDomElement ele = nodes.at(i).toElement();
            if (ele.tagName() == TagName_Name)
                nameFromDom(ele);
            else if (ele.tagName() == TagName_Visibility)
                visibilityFromDom(ele);
            else if (ele.tagName() == TagName_Open)
                openFromDom(ele);
            else if (ele.tagName() == TagName_Address)
                addressFromDom(ele);
            else if (ele.tagName() == TagName_Description)
                descriptionFromDom(ele);
            else if (ele.tagName() == "Style")
                styleFromDom(ele);
            else if (ele.tagName() == TagName_Placemark)
            {
                KMLFeature *f = new KMLPlacemark;
                f->fromDom(ele);
                mFeatures.push_back(f);
            }
            else if (ele.tagName() == TagName_Document)
            {
                KMLFeature *f = new KMLDocument;
                f->fromDom(ele);
                mFeatures.push_back(f);
            }
            else if (ele.tagName() == TagName_Folder)
            {
                KMLFeature *f = new KMLFolder;
                f->fromDom(ele);
                mFeatures.push_back(f);
            }
        }
    }
    return true;
}

KMLFolder::KMLFolder(const QString &id)
    : KMLFeature(id)
{
}

QVector<KMLFeature *> KMLFolder::features() const
{
    return mFeatures;
}

void KMLFolder::setFeatures(const QVector<KMLFeature *> &features)
{
    mFeatures = features;
}

void KMLFolder::deleteAll()
{
    qDeleteAll(mFeatures);
    mFeatures.clear();
}

QDomElement KMLFolder::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement(TagName_Document);
    element.appendChild(nameToDom());
    element.appendChild(visibilityToDom());
    element.appendChild(openToDom());
    element.appendChild(addressToDom());
    element.appendChild(descriptionToDom());
    element.appendChild(abstractViewToDom());
#ifdef UNUSED
    element.appendChild(styleUrlToDom());
#endif
    element.appendChild(styleToDom());

    for (int i = 0; i < mFeatures.size(); ++i)
    {
        if (mFeatures.at(i))
            element.appendChild(mFeatures.at(i)->toDom());
    }

    return element;
}

bool KMLFolder::fromDom(const QDomElement &dom)
{
    QDomNodeList nodes = dom.childNodes();
    if (!nodes.isEmpty())
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            QDomElement ele = nodes.at(i).toElement();
            if (ele.tagName() == TagName_Name)
                nameFromDom(ele);
            else if (ele.tagName() == TagName_Visibility)
                visibilityFromDom(ele);
            else if (ele.tagName() == TagName_Open)
                openFromDom(ele);
            else if (ele.tagName() == TagName_Address)
                addressFromDom(ele);
            else if (ele.tagName() == TagName_Description)
                descriptionFromDom(ele);
            else if (ele.tagName() == "Style")
                styleFromDom(ele);
            else if (ele.tagName() == TagName_Placemark)
            {
                KMLFeature *f = new KMLPlacemark;
                f->fromDom(ele);
                mFeatures.push_back(f);
            }
            else if (ele.tagName() == TagName_Document)
            {
                KMLFeature *f = new KMLDocument;
                f->fromDom(ele);
                mFeatures.push_back(f);
            }
            else if (ele.tagName() == TagName_Folder)
            {
                KMLFeature *f = new KMLFolder;
                f->fromDom(ele);
                mFeatures.push_back(f);
            }
        }
    }
    return true;
}
