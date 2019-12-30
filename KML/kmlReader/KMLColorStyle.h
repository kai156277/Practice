#pragma once   // KMLCOLORSTYLE_H

#include "KMLObject.h"

#include <QColor>
#include <QIcon>

class KMLColorStyle : public KMLObject
{
public:
    enum Type
    {
        Line = 0,
        Poly,
        Icon
    };

    KMLColorStyle(Type type, const QColor &color = Qt::white, const QString &id = QString());

    Type type() const;
    void setType(const Type &type);

    QColor color() const;
    void   setColor(const QColor &color);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    Type   mType;
    QColor mColor;
};

class KMLLineStyle : public KMLColorStyle
{
public:
    KMLLineStyle(double width = 1, const QColor &color = Qt::white, const QString &id = QString());

    int  width() const;
    void setWidth(int width);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    int mWidth;
};

class KMLPolyStyle : public KMLColorStyle
{
public:
    KMLPolyStyle(bool fill = true, bool outline = false, const QColor &color = Qt::white, const QString &id = QString());

    bool fill() const;
    void setFill(bool fill);

    bool outLine() const;
    void setOutLine(bool outLine);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    bool mFill;
    bool mOutLine;
};

// TODO: 暂不使用
#ifdef UNUSED
class KMLIconStyle : public KMLColorStyle
{
public:
    KMLIconStyle(const QIcon &icon = QIcon(), float scale = 1, const QColor &color = Qt::white, const QString &id = QString());

    float scale() const;
    void  setScale(float scale);

    QIcon icon() const;
    void  setIcon(const QIcon &icon);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    float   mScale;
    KMLIcon mIcon;
};
#endif
