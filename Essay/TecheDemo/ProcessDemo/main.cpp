#include "Demo.h"

#include <QApplication>
#include <QDebug>
#include <QtCore>
#include <QtWidgets>

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

static QFile *slog;

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageOutput);
    //    QString QT_MESSAGE_PATTERN =
    //        "[%{type} "
    //        "%{threadid} "
    //        "%{time yyyyMMdd hhmmss }"
    //        "%{function}] "
    //        "%{message}";

    //    qSetMessagePattern(QT_MESSAGE_PATTERN);
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("ProcessDemo");
    QCoreApplication::setOrganizationName("XiuShan");
    QCoreApplication::setOrganizationDomain("supersurs.com");
    QDir home(QDir::homePath());
    home.mkdir("ProcessDemo");
    QString logFile = QDir::homePath() + "/ProcessDemo/" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss.log");
    slog            = new QFile(logFile);
    if (!slog->open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr, "Error!", "Log File Can`t open");
        exit(1);
    }
    Demo w;
    w.show();
    qDebug() << "main thread";

    int ret = a.exec();
    slog->close();
    return ret;
}

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream write(slog);
    write << msg << endl;
}
