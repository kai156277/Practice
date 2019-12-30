#include "Canvas.h"

#include "ui_Canvas.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <algorithm>

#pragma execution_character_set("utf-8")
#define _PI 3.141592653
#define _2PI 6.283185307

double distance(const QPoint &a, const QPoint &b)
{
    int dx = a.x() - b.x();
    int dy = a.y() - b.y();
    return sqrt(dx * dx + dy * dy);
}

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Canvas)
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    setMouseTracking(true);
}

Canvas::~Canvas()
{
    delete ui;
}

void Canvas::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    mMousePos = event->pos();
    menu.addAction("添加点", this, &Canvas::slotAddPoint);
    menu.addAction("计算凸包", this, &Canvas::slotGetConyexHull);
    menu.addAction("清除点", this, &Canvas::slotClearPoint);
    menu.exec(QCursor::pos());
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 绘制点
    painter.save();
    painter.setBrush(Qt::black);
    for (int i = 0; i < mPoints.size(); ++i)
    {
        QPoint  point   = mPoints.at(i);
        QString posText = QString("Point_%1(%2, %3)").arg(i).arg(point.x()).arg(point.y());
        painter.drawEllipse(point, 2, 2);
        painter.drawText(point + QPoint(4, 0), posText);
    }
    painter.restore();

    // 绘制边界矩形
    painter.save();
    painter.setPen(QPen(QBrush(Qt::red), 2));
    painter.drawRect(mPoints.boundingRect());
    painter.restore();

    // 绘制扫描线
    painter.save();
    painter.setPen(QPen(QBrush(Qt::blue), 2, Qt::DashLine));
    for (int i = 0; i < mPoints.size() - 1; ++i)
    {
        painter.drawLine(mPoints.last(), mPoints.at(i));
    }
    painter.restore();

    // 绘制凸包
    painter.save();
    painter.setPen(QPen(QBrush(Qt::green), 2, Qt::DashLine));
    painter.drawConvexPolygon(mConyexHullPoints);
    painter.restore();
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QString posText = QString("Point(%1, %2)").arg(event->pos().x()).arg(event->pos().y());
    setWindowTitle(posText);
}

void Canvas::slotAddPoint()
{
    mPoints.append(mMousePos);

    // 按y的最小值排序
    auto yMinCompare = [](const QPoint &a, const QPoint &b) -> bool {
        if (a.y() == b.y())
            return a.x() < b.x();
        else
            return a.y() < b.y();
    };
    std::sort(mPoints.begin(),
              mPoints.end(),
              yMinCompare);

    QPoint last = mPoints.last();

    // 极角排序
    auto polarAngleCompare = [last](const QPoint &a, const QPoint &b) -> bool {
        double polarA = atan2((a.y() - last.y()), (a.x() - last.x()));
        double polarB = atan2((b.y() - last.y()), (b.x() - last.x()));

        if (polarA != polarB)
            return polarA < polarB;
        else
            return a.x() < b.x();
    };
    std::sort(mPoints.begin(),
              mPoints.end(),
              polarAngleCompare);
    update();
}

void Canvas::slotGetConyexHull()
{
    // 判断ab向量到bc向量的角度（逆时针），左转小于180°
    auto cross = [](const QPoint &a, const QPoint &b, const QPoint &c) -> bool {
        // NOTE: 角度计算的时候：
        // 1. 选好旋转方向
        // 2. 多画图
        QPoint bc = c - b;
        QPoint ab = b - a;

        double angleA = atan2(-ab.y(), ab.x());
        double angleB = atan2(-bc.y(), bc.x());

        angleA = angleA < 0 ? (angleA + _2PI) : angleA;
        angleB = angleB < 0 ? (angleB + _2PI) : angleB;

        if (angleA > angleB)
            angleB = angleB + _2PI;

        return (angleB - angleA) < _PI;
    };

    int last = mPoints.size() - 1;
    mConyexHullPoints.clear();
    mConyexHullPoints.reserve(mPoints.size());
    mConyexHullPoints.push_back(mPoints.at(last));
    mConyexHullPoints.push_back(mPoints.at(last - 1));
    for (int i = last - 2; i > 0; --i)
    {
        int    size = mConyexHullPoints.size();
        QPoint a    = mConyexHullPoints.at(size - 2);
        while (!cross(a, mConyexHullPoints.last(), mPoints.at(i)))
        {
            mConyexHullPoints.removeLast();
            size = mConyexHullPoints.size();
            a    = mConyexHullPoints.at(size - 2);
        }
        mConyexHullPoints.push_back(mPoints.at(i));
    }
    mConyexHullPoints.push_back(mPoints.at(0));

    qDebug() << "process last";
    int size = mConyexHullPoints.size();
    while (!cross(mConyexHullPoints.at(size - 3), mConyexHullPoints.at(size - 2), mConyexHullPoints.at(size - 1)))
    {
        qDebug() << "last 2:" << mConyexHullPoints.at(size - 2);
        mConyexHullPoints.removeAt(size - 2);
        size = mConyexHullPoints.size();
    }

    update();
}

void Canvas::slotClearPoint()
{
    mPoints.clear();
    mConyexHullPoints.clear();
    update();
}
