
#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "KMLColorStyle.h"

#include <QDomNode>

KMLColorStyle::KMLColorStyle(Type type, const QColor &color, const QString &id)
    : KMLObject(id)
    , mType(type)
    , mColor(color)
{
}

KMLColorStyle::Type KMLColorStyle::type() const
{
    return mType;
}

void KMLColorStyle::setType(const Type &type)
{
    mType = type;
}

QColor KMLColorStyle::color() const
{
    return mColor;
}

void KMLColorStyle::setColor(const QColor &color)
{
    mColor = color;
}

QDomElement KMLColorStyle::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("color");
    if (color().isValid())
    {
        int      r    = color().red();
        int      g    = color().green();
        int      b    = color().blue();
        int      a    = color().alpha();
        QDomText text = doc.createTextNode(QString::asprintf("%.2x%.2x%.2x%.2x", a, b, g, r));
        element.appendChild(text);
    }
    return element;
}

bool KMLColorStyle::fromDom(const QDomElement &dom)
{
    if (dom.hasChildNodes())
    {
        QDomNodeList colors = dom.elementsByTagName("color");
        if (!colors.isEmpty())
        {
            QString colorStr = colors.item(0).toElement().text();
            bool    ok       = false;

            int a = colorStr.mid(0, 2).toInt(&ok, 16);
            if (!ok)
                return false;
            int b = colorStr.mid(2, 2).toInt(&ok, 16);
            if (!ok)
                return false;
            int g = colorStr.mid(4, 2).toInt(&ok, 16);
            if (!ok)
                return false;
            int r = colorStr.mid(6, 2).toInt(&ok, 16);

            QColor color(r, g, b, a);
            setColor(color);
        }
    }
    return true;
}

KMLLineStyle::KMLLineStyle(double width, const QColor &color, const QString &id)
    : KMLColorStyle(Line, color, id)
    , mWidth(width)
{
}

int KMLLineStyle::width() const
{
    return mWidth;
}

void KMLLineStyle::setWidth(int width)
{
    mWidth = width;
}

QDomElement KMLLineStyle::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("LineStyle");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    element.appendChild(KMLColorStyle::toDom());
    {
        QDomElement ele  = doc.createElement("width");
        QDomText    text = doc.createTextNode(QString::number(mWidth));
        ele.appendChild(text);
        element.appendChild(ele);
    }

    return element;
}

bool KMLLineStyle::fromDom(const QDomElement &dom)
{
    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {

        if (!KMLColorStyle::fromDom(dom))
            return false;

        QDomNodeList widths = dom.elementsByTagName("width");
        if (!widths.isEmpty())
        {
            QString widthStr = widths.item(0).toElement().text();
            bool    ok       = false;

            int w = widthStr.toInt(&ok);
            if (!ok)
                return false;

            setWidth(w);
        }
    }
    return true;
}

KMLPolyStyle::KMLPolyStyle(bool fill, bool outline, const QColor &color, const QString &id)
    : KMLColorStyle(Poly, color, id)
    , mFill(fill)
    , mOutLine(outline)
{
}

bool KMLPolyStyle::fill() const
{
    return mFill;
}

void KMLPolyStyle::setFill(bool fill)
{
    mFill = fill;
}

bool KMLPolyStyle::outLine() const
{
    return mOutLine;
}

void KMLPolyStyle::setOutLine(bool outLine)
{
    mOutLine = outLine;
}

QDomElement KMLPolyStyle::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("PolyStyle");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    element.appendChild(KMLColorStyle::toDom());
    {
        {
            QDomElement ele  = doc.createElement("fill");
            QDomText    text = doc.createTextNode(QString::number(mFill ? 1 : 0));
            ele.appendChild(text);
            element.appendChild(ele);
        }
        {
            QDomElement ele  = doc.createElement("outline");
            QDomText    text = doc.createTextNode(QString::number(mOutLine ? 1 : 0));
            ele.appendChild(text);
            element.appendChild(ele);
        }
    }

    return element;
}

bool KMLPolyStyle::fromDom(const QDomElement &dom)
{
    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {

        if (!KMLColorStyle::fromDom(dom))
            return false;

        QDomNodeList fills = dom.elementsByTagName("fill");
        if (!fills.isEmpty())
        {
            QString fillStr = fills.item(0).toElement().text();
            bool    ok      = false;

            int fill = fillStr.toInt(&ok);
            if (!ok)
                return false;

            setFill((fill == 1));
        }

        QDomNodeList outlines = dom.elementsByTagName("outline");
        if (!outlines.isEmpty())
        {
            QString outlineStr = outlines.item(0).toElement().text();
            bool    ok         = false;

            int outline = outlineStr.toInt(&ok);
            if (!ok)
                return false;

            setOutLine((outline == 1));
        }
    }
    return true;
}

#ifdef UNUSED
KMLIconStyle::KMLIconStyle(const QIcon &icon, float scale, const QColor &color, const QString &id)
    : KMLColorStyle(Icon, color, id)
    , mScale(scale)
    , mIcon(icon)
{
}

float KMLIconStyle::scale() const
{
    return mScale;
}

void KMLIconStyle::setScale(float scale)
{
    mScale = scale;
}

QIcon KMLIconStyle::icon() const
{
    return mIcon.icon();
}

void KMLIconStyle::setIcon(const QIcon &icon)
{
    mIcon.setIcon(icon);
}

QDomElement KMLIconStyle::toDom() const
{
    QDomDocument doc;
    QDomElement  element = doc.createElement("IconStyle");
    if (!id().isEmpty())
    {
        element.setAttribute("id", id());
    }

    element.appendChild(KMLColorStyle::toDom());
    {
        QDomElement ele  = doc.createElement("scale");
        QDomText    text = doc.createTextNode(QString::number(mScale, 'f', 6));
        ele.appendChild(text);
        element.appendChild(ele);
    }
    element.appendChild(mIcon.toDom());

    return element;
}

bool KMLIconStyle::fromDom(const QDomElement &dom)
{
    if (dom.hasAttribute("id"))
    {
        setId(dom.attribute("id"));
    }

    if (dom.hasChildNodes())
    {

        if (!KMLColorStyle::fromDom(dom))
            return false;

        if (!mIcon.fromDom(dom))
            return false;

        QDomNodeList scales = dom.elementsByTagName("scale");
        if (!scales.isEmpty())
        {
            QString scaleStr = scales.item(0).toElement().text();
            bool    ok       = false;

            int fill = scaleStr.toInt(&ok);
            if (!ok)
                return false;

            setScale(fill);
        }
    }
    return true;
}

#endif
