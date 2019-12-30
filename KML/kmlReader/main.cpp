#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

#include "KMLAbstractView.h"
#include "KMLColorStyle.h"
#include "KMLFeature.h"
#include "KMLGeometry.h"
#include "KMLObject.h"
#include "Widget.h"

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtXml>
#include <algorithm>
#include <thread>

#pragma execution_character_set("utf-8")

void checkPoint();

void displayFeature(KMLFeature *f);

//void mallocMemory(char **ptr, uint64_t size)
//{
//    char *ptr1 = new char[size];

//    char txt[] = "Hello world!";
//    qDebug() << "sizeof:" << sizeof(txt);
//    memcpy(ptr1, txt, sizeof(txt));
//    printf("ptr1 context: %s\n", ptr1);
//    (*ptr) = ptr1;
//}
//    char *ptr = nullptr;
//    mallocMemory(&ptr, 20);
//    printf("ptr context: %s\n", ptr);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVector<int> values = {1, 2, 2, 2, 3, 3, 4, 4};
    auto         upper  = std::upper_bound(values.begin(), values.end(), 4);
    qDebug() << "upper:" << *upper;
    values.insert(upper, 9);
    qDebug() << values;
    try
    {
        throw std::out_of_range("超出范围");
    }
    catch (const std::exception &e)
    {
        qDebug() << e.what();
    }

#if 0
    checkPoint();
    QDomDocument doc;
    QDomElement  element = doc.createElement("name");
    QDomText     text    = doc.createTextNode("hello");
    element.appendChild(text);
    QString     output;
    QTextStream stream(&output);
    element.save(stream, 0);

    qDebug() << output;
    QString file = QFileDialog::getOpenFileName(nullptr, "kml", "C:\\Users\\zhaokai\\Desktop", "kml (*.kml)");

    if (file.isEmpty())
        return 0;

    QDomDocument doc;

    QFile kmlfile(file);

    if (!kmlfile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error Open kml file";
        return 0;
    }

    if (!doc.setContent(&kmlfile))
    {
        kmlfile.close();
        return 0;
    }
    kmlfile.close();

    QDomElement kmlEle = doc.documentElement();
    qDebug() << "doc Elem tag name:" << kmlEle.tagName();
    if (kmlEle.tagName() == "kml")
    {
        KMLDocument kmlDoc;
        kmlDoc.fromDom(kmlEle.firstChildElement());
        displayFeature(&kmlDoc);
    }
    return 0;
#endif
}

void checkPoint()
{
    QDir  dir(QCoreApplication::applicationDirPath() + "/UnitTest/Geometry");
    QFile check(QCoreApplication::applicationDirPath() + "/UnitTest/Geometry/check.txt");
    if (!check.open(QIODevice::ReadOnly))
    {
        qDebug() << "open checkfile faild";
        return;
    }
    QMap<QString, QStringList> checklines;
    while (!check.atEnd())
    {
        QString line = check.readLine();
        if (line.startsWith("#"))
            continue;

        QStringList tmp = line.split(",");
        checklines.insert(tmp.at(0), tmp);
    }
    QStringList kml;
    kml << "*.kml";
    QFileInfoList list = dir.entryInfoList(kml, QDir::Files);
    for (const QFileInfo &info : list)
    {
        QDomDocument doc;
        QFile        kmlfile(info.filePath());
        if (!kmlfile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Error Open" << info.filePath();
            continue;
        }
        if (!doc.setContent(&kmlfile))
        {
            kmlfile.close();
            continue;
        }
        kmlfile.close();

        QDomElement docElem = doc.documentElement();
        KMLPoint    point;
        bool        flag = point.fromDom(docElem);
        if (checklines.contains(info.baseName()))
        {
            QStringList tmp = checklines.value(info.baseName());
            qDebug() << info.baseName() << flag << point.id() << point.coord().mLongitude
                     << point.coord().mLatitude << point.coord().mAltitude;
            qDebug() << "checklist:" << tmp;
            qDebug() << "fromDom check:" << (flag == (tmp.at(1) == "true"));
            qDebug() << "id:" << (point.id() == tmp.at(2));
            qDebug() << "lng:" << (abs(point.coord().mLongitude - tmp.at(3).toDouble()) < 1e-6);
            qDebug() << "lat:" << (abs(point.coord().mLatitude - tmp.at(4).toDouble()) < 1e-6);
            qDebug() << "alt:" << (abs(point.coord().mAltitude - tmp.at(5).toDouble()) < 1e-6);
        }
        else
        {
            qDebug() << "error : filename" << info.fileName();
        }
    }
}

void displayFeature(KMLFeature *f)
{
    qDebug() << "---------------------------";
    qDebug() << "name:" << f->name();
    qDebug() << "visibility:" << f->visibility();
    qDebug() << "open:" << f->open();
    qDebug() << "address:" << f->address();
    qDebug() << "description:" << f->description();
    KMLStyle style = f->style();
    for (int i = 0; i < style.count(); ++i)
    {
        KMLColorStyle::Type type = style.at(i)->type();
        qDebug() << "style type:" << type;
        qDebug() << "style color:" << style.at(i)->color();
        switch (type)
        {
        case KMLColorStyle::Line:
        {
            KMLLineStyle *lineStyle = dynamic_cast<KMLLineStyle *>(style.at(i));
            qDebug() << "line style width:" << lineStyle->width();
            break;
        }
        case KMLColorStyle::Poly:
        {
            KMLPolyStyle *polyStyle = dynamic_cast<KMLPolyStyle *>(style.at(i));
            qDebug() << "poly style fill:" << polyStyle->fill();
            qDebug() << "poly style outline:" << polyStyle->outLine();
            break;
        }
        default:
            break;
        }
    }
    KMLPlacemark *p = dynamic_cast<KMLPlacemark *>(f);
    if (p)
    {
        qDebug() << "Placemark:";
        KMLGeometry::Type t = p->geometry()->type();
        switch (t)
        {
        case KMLGeometry::Point:
        {
            KMLPoint *point = dynamic_cast<KMLPoint *>(p->geometry());
            qDebug() << "point:";
            qDebug("%.10lf, %.10lf, %.10lf ",
                   point->coord().mLongitude,
                   point->coord().mLatitude,
                   point->coord().mAltitude);
            break;
        }
        case KMLGeometry::LinearRing:
        {
            qDebug() << "KMLLinearRing:";
            KMLLinearRing *linear = dynamic_cast<KMLLinearRing *>(p->geometry());
            KMLCoordinates coords = linear->coords();
            for (int i = 0; i < coords.size(); i++)
            {
                qDebug("%.10lf, %.10lf, %.10lf ",
                       coords.at(i).mLongitude,
                       coords.at(i).mLatitude,
                       coords.at(i).mAltitude);
            }
            break;
        }
        case KMLGeometry::LineString:
        {
            qDebug() << "KMLLineString:";
            KMLLineString *linear = dynamic_cast<KMLLineString *>(p->geometry());
            KMLCoordinates coords = linear->coords();
            for (int i = 0; i < coords.size(); i++)
            {
                qDebug("%.10lf, %.10lf, %.10lf ",
                       coords.at(i).mLongitude,
                       coords.at(i).mLatitude,
                       coords.at(i).mAltitude);
            }
            break;
        }
        case KMLGeometry::Polygon:
        {
            qDebug() << "Polygon:";
            KMLPolygon *   polygon = dynamic_cast<KMLPolygon *>(p->geometry());
            KMLLinearRing  outer   = polygon->outerBoundary();
            KMLCoordinates coords  = outer.coords();
            for (int i = 0; i < coords.size(); i++)
            {
                qDebug("%.10lf, %.10lf, %.10lf",
                       coords.at(i).mLongitude,
                       coords.at(i).mLatitude,
                       coords.at(i).mAltitude);
            }
            break;
        }
        default:
            break;
        }
        return;
    }

    KMLDocument *d = dynamic_cast<KMLDocument *>(f);
    if (d)
    {
        qDebug() << "document:";
        QVector<KMLFeature *> features = d->features();
        for (int i = 0; i < features.size(); ++i)
        {
            displayFeature(features.at(i));
        }
    }

    KMLFolder *folder = dynamic_cast<KMLFolder *>(f);
    if (folder)
    {
        qDebug() << "folder:";
        QVector<KMLFeature *> features = d->features();
        for (int i = 0; i < features.size(); ++i)
        {
            displayFeature(features.at(i));
        }
    }
}
