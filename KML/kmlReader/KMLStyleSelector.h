#pragma once   // KMLSTYLESELECTOR_H

#include "KMLColorStyle.h"
#include "KMLObject.h"

#include <QPolygon>
class KMLStyleSelector : public KMLObject
{
public:
    enum Type
    {
        Style = 0,
        StyleMap
    };

    KMLStyleSelector(Type type, const QString &id = QString());

    Type type() const;
    void setType(const Type &type);

private:
    Type mType;
};

class KMLStyle : public KMLStyleSelector
{
public:
    KMLStyle(const QString &id = QString());

    void addStyle(KMLColorStyle *style);
    void removeStyle(int index);
    int  count() const;

    KMLColorStyle *at(int index);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    QVector<KMLColorStyle *> mStyles;
};

class KMLStyleMap : public KMLStyleSelector
{
public:
private:
};
