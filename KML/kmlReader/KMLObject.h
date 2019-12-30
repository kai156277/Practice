#pragma once   // KMLDEF_H

#include <QDomElement>
#include <QIcon>
#include <QString>

class KMLObject
{
public:
    KMLObject(const QString &id = QString());
    virtual ~KMLObject();
    QString id() const;
    void    setId(const QString &_id);

    virtual QDomElement toDom() const                   = 0;
    virtual bool        fromDom(const QDomElement &dom) = 0;

private:
    QString  mId;
    QDomNode mNode;
};

class KMLIcon : public KMLObject
{
public:
    enum RefreshModeType
    {
        OnChange = 0,
        OnInterval,
        OnExpire
    };
    static QString RefreshModeTypeToString(RefreshModeType type);

    KMLIcon(const QIcon &icon = QIcon(), RefreshModeType type = OnChange, int interval = 0, const QString &id = QString());

    QIcon icon() const;
    void  setIcon(const QString &iconPath);

    RefreshModeType refreshMode() const;
    void            setRefreshMode(const RefreshModeType &refreshMode);

    int  refreshInterval() const;
    void setRefreshInterval(int refreshInterval);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    QString         mIconPath;
    RefreshModeType mRefreshMode;
    int             mRefreshInterval;
};

class KMLLocation : public KMLObject
{
public:
    KMLLocation() = default;
    KMLLocation(double lng, double lat, double alt, const QString &id = QString());

    double longitude() const;
    void   setLongitude(double longitude);

    double latitude() const;
    void   setLatitude(double latitude);

    double altitude() const;
    void   setAltitude(double altitude);

    QDomElement toDom() const override;
    bool        fromDom(const QDomElement &dom) override;

private:
    double mLongitude;
    double mLatitude;
    double mAltitude;
};

struct KMLCoordinate
{
    double mLongitude = 0;
    double mLatitude  = 0;
    double mAltitude  = 0;
};

class KMLCoordinates : public QVector<KMLCoordinate>
{
public:
    QDomElement toDom() const;
    bool        fromDom(const QDomElement &dom);
};

// TODO: 不能使用
#ifdef UNUSED
class KMLStyleUrl
{
public:
    QDomElement toDom() const;
    bool        fromDom(const QDomElement &dom);
};
#endif
