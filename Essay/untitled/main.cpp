#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "Widget.h"

#include <QApplication>
#include <QDebug>
#include <QtCore>
#include <QtWidgets>
#include <proj.h>

QString getProjFromFile(const QString &file, short num)
{
    QStringList list;
    QFile       proj(file);
    if (!proj.open(QIODevice::ReadOnly))
    {
        qDebug() << file << "File Open Faild";
    }

    QTextStream write(&proj);
    QString     text;
    text.resize(120);
    while (!write.atEnd())
    {
        write.readLineInto(&text);
        if (text.contains(QString("<%1>").arg(num)))
        {
            list = text.split(" ");
            break;
        }
    }
    proj.close();
    list.removeFirst();
    list.removeLast();
    return list.join(" ");
}

QString getParam(QString one, QString two, QTextStream *read)
{
    QStringList list;
    if (one.contains("#") && two.contains("<>") && !two.contains("#"))
    {
        list = two.split(" ");
        list.removeLast();
        one.remove(QChar('#'));
        list.push_back(QString("<%1>").arg(one.trimmed()));
        return list.join(" ");
    }
    else
    {
        one = two;
        two = read->readLine();
        return getParam(one, two, read);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString      epsgFile = QFileDialog::getOpenFileName(nullptr, "proj4");
    if (epsgFile.isEmpty())
    {
        qDebug() << "Open File Faild";
        return 0;
    }
    QString newEpsgFile = epsgFile + "-new.txt";
    QFile   newfile(newEpsgFile);
    newfile.open(QIODevice::WriteOnly);
    QTextStream write(&newfile);

    QFile       file(epsgFile);
    QString     firstLine;
    QString     secondLine;
    QStringList list;
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream read(&file);

        while (!read.atEnd())
        {
            firstLine  = read.readLine();
            secondLine = read.readLine();
            write << getParam(firstLine, secondLine, &read) << endl;
        }
    }

#if 0

    QString epsgFile = QFileDialog::getOpenFileName(nullptr, "proj4");

    QFile   file(epsgFile);
    QString line;

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream read(&file);

        QFile file1(epsgFile + ".check");
        file1.open(QIODevice::WriteOnly);
        QTextStream write(&file1);

        while (!read.atEnd())
        {
            line = read.readLine();
            if (!line.contains("#"))
            {
                write << line << endl;
            }
        }
    }
#endif

    qDebug() << "End";
    return 0;
}

//    while (true)
//    {
//        PJ_CONTEXT *context = proj_context_create();

//        //    QString epsgPath = "epsg:32651";
//        //        QString file = QFileDialog::getOpenFileName(nullptr, "proj4");
//        QString file = QApplication::applicationDirPath() + "/share/proj/epsg";
//        if (file.isEmpty())
//            return 0;

//        //        int num = QInputDialog::getInt(nullptr, "proj4 num", "num");
//        int num = 32651;

//        QString projStr = getProjFromFile(file, num);
//        qDebug() << "epsg:" << projStr;
//        PJ *         proj = proj_create(context, projStr.toLocal8Bit().data());
//        PJ_PROJ_INFO info = proj_pj_info(proj);
//        qDebug() << "proj:" << info.definition;
//        qDebug() << "proj description:" << info.description;
//        qDebug() << "proj id:" << info.id;

//        proj_destroy(proj);
//        proj_context_destroy(context);
//        getchar();
//    }
