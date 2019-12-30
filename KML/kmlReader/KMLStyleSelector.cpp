
#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "KMLStyleSelector.h"

KMLStyleSelector::KMLStyleSelector(KMLStyleSelector::Type type, const QString &id)
    : KMLObject(id)
    , mType(type)
{
}

KMLStyleSelector::Type KMLStyleSelector::type() const
{
    return mType;
}

void KMLStyleSelector::setType(const Type &type)
{
    mType = type;
}

KMLStyle::KMLStyle(const QString &id)
    : KMLStyleSelector(Style, id)
{
}

void KMLStyle::addStyle(KMLColorStyle *style)
{
    mStyles.push_back(style);
}

void KMLStyle::removeStyle(int index)
{
    if (0 <= index && index < mStyles.size())
        mStyles.removeAt(index);
}

int KMLStyle::count() const
{
    return mStyles.count();
}

KMLColorStyle *KMLStyle::at(int index)
{
    return mStyles.value(index);
}

QDomElement KMLStyle::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("Style");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    for (int i = 0; i < mStyles.count(); ++i)
    {
        element.appendChild(mStyles.at(i)->toDom());
    }

    return element;
}

bool KMLStyle::fromDom(const QDomElement &dom)
{
    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {
#ifdef UNUSED
        {
            QDomNodeList styles = dom.elementsByTagName("IconStyle");
            if (!styles.isEmpty())
            {
                for (int i = 0; i < styles.size(); ++i)
                {
                    QDomElement    ele   = styles.item(i).toElement();
                    KMLColorStyle *style = new KMLIconStyle();
                    style->fromDom(ele);
                    mStyles.push_back(style);
                }
            }
        }
#endif
        {
            QDomNodeList styles = dom.elementsByTagName("LineStyle");
            if (!styles.isEmpty())
            {
                for (int i = 0; i < styles.size(); ++i)
                {
                    QDomElement    ele   = styles.item(i).toElement();
                    KMLColorStyle *style = new KMLLineStyle();
                    style->fromDom(ele);
                    mStyles.push_back(style);
                }
            }
        }
        {
            QDomNodeList styles = dom.elementsByTagName("PolyStyle");
            if (!styles.isEmpty())
            {
                for (int i = 0; i < styles.size(); ++i)
                {
                    QDomElement    ele   = styles.item(i).toElement();
                    KMLColorStyle *style = new KMLPolyStyle();
                    style->fromDom(ele);
                    mStyles.push_back(style);
                }
            }
        }
    }
    return true;
}
