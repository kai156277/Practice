#pragma once   // FTPCLIENT_H
#include <QObject>

class QUrl;
class QFile;
class QNetworkReply;
class QNetworkAccessManager;

class FtpClient : public QObject
{
    Q_OBJECT
public:
    explicit FtpClient(QObject *parent = nullptr);
    void ftpGet(const QString &source, const QString &dev);
    void ftpPut(const QString &source, const QString &dev);

private slots:
    void finished(QNetworkReply *reply);

private:
    QUrl *                 mUrl;
    QFile *                mFile;
    QNetworkReply *        mReply;
    QNetworkAccessManager *mManager;
};
