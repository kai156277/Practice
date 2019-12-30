#include "FtpClient.h"

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

FtpClient::FtpClient(QObject *parent)
    : QObject(parent)
{
    mManager = new QNetworkAccessManager();
    mUrl     = new QUrl();
    mUrl->setScheme("ftp");
    connect(mManager, &QNetworkAccessManager::finished, this, &FtpClient::finished);
}

void FtpClient::ftpGet(const QString &source, const QString &dev)
{
}

void FtpClient::ftpPut(const QString &source, const QString &dev)
{
}

void FtpClient::finished(QNetworkReply *reply)
{
    mFile->write(reply->readAll());
    mFile->flush();
    mFile->close();
    reply->deleteLater();
}
