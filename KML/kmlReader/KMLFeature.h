#pragma once   // KMLFEATURE_H

#include "KMLObject.h"
#include "KMLStyleSelector.h"

class KMLAbstractView;
class KMLStyleSelector;
class KMLFeature : public KMLObject
{
public:
    KMLFeature(const QString &id = QString());

    // "<name>hello</name>
    QString     name() const;
    void        setName(const QString &name);
    QDomElement nameToDom() const;
    bool        nameFromDom(const QDomElement &dom);

    bool        visibility() const;
    void        setVisibility(bool visibility);
    QDomElement visibilityToDom() const;
    bool        visibilityFromDom(const QDomElement &dom);

    bool        open() const;
    void        setOpen(bool open);
    QDomElement openToDom() const;
    bool        openFromDom(const QDomElement &dom);

    QString     address() const;
    void        setAddress(const QString &address);
    QDomElement addressToDom() const;
    bool        addressFromDom(const QDomElement &dom);

    QString     description() const;
    void        setDescription(const QString &description);
    QDomElement descriptionToDom() const;
    bool        descriptionFromDom(const QDomElement &dom);

    KMLAbstractView *abstractView() const;
    void             setAbstractView(KMLAbstractView *abstractView);
    QDomElement      abstractViewToDom() const;
    bool             abstractViewFromDom(const QDomElement &dom);

    // TODO: KMLStyleUrl不能使用
#ifdef UNUSED
    KMLStyleUrl styleUrl() const;
    void        setStyleUrl(const KMLStyleUrl &styleUrl);
    QDomElement styleUrlToDom() const;
    bool        styleUrlFromDom(const QDomElement &dom);
    KMLStyleUrl mStyleUrl;
#endif

    KMLStyle    style() const;
    void        setStyle(const KMLStyle &style);
    QDomElement styleToDom() const;
    bool        styleFromDom(const QDomElement &dom);

private:
    bool             mVisibility = true;
    bool             mOpen       = true;
    QString          mName;
    QString          mAddress;
    QString          mDescription;
    KMLAbstractView *mAbstractView = nullptr;
    KMLStyle         mStyle;
};

class KMLGeometry;
class KMLPlacemark : public KMLFeature
{
public:
    KMLPlacemark(const QString &id = QString());

    KMLGeometry *geometry() const;
    void         setGeometry(KMLGeometry *geometry);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    KMLGeometry *mGeometry = nullptr;
};

class KMLDocument : public KMLFeature
{
public:
    KMLDocument(const QString &id = QString());

    QVector<KMLFeature *> features() const;
    void                  setFeatures(const QVector<KMLFeature *> &features);
    void                  deleteAll();

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    QVector<KMLFeature *> mFeatures;
};

class KMLFolder : public KMLFeature
{
public:
    KMLFolder(const QString &id = QString());

    QVector<KMLFeature *> features() const;
    void                  setFeatures(const QVector<KMLFeature *> &features);
    void                  deleteAll();

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    QVector<KMLFeature *> mFeatures;
};
