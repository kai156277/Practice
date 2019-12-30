#include "AnalogClock.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QString::fromStdWString(L"模拟计时器"));
    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

void AnalogClock::paintEvent(QPaintEvent *event)
{
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)};

    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -60)};

    static const QPoint secondHand[3] = {
        QPoint(5, 8),
        QPoint(-5, 8),
        QPoint(0, -80)

    };

    static const QRectF txtRectangle(-5, -5, 10, 10);

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);

    int   side = qMin(width(), height());
    QTime time = QTime::currentTime();

    // 转换QPainter坐标系
    QPainter painter(this);
    painter.setFont(QFont(QApplication::font().family(), 8));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // 绘制时针
    painter.save();
    painter.setPen(hourColor);
    painter.setBrush(hourColor);
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();

    // 绘制分针
    painter.save();
    painter.setPen(minuteColor);
    painter.setBrush(minuteColor);
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    // 绘制秒针
    painter.save();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.rotate(6.0 * time.second());
    painter.drawConvexPolygon(secondHand, 3);
    painter.restore();

    // 绘制刻度
    painter.save();
    for (int j = 0; j < 60; ++j)
    {
        if ((j % 5) != 0)
        {
            painter.setPen(minuteColor);
            painter.drawLine(0, -92, 0, -96);
        }
        else
        {
            painter.setPen(hourColor);
            painter.drawLine(0, -90, 0, -96);
        }
        painter.rotate(6.0);
    }
    painter.restore();

    // 绘制计数
    for (int j = 1; j <= 12; ++j)
    {
        painter.save();
        painter.rotate(j * 30);
        painter.translate(0, -80);
        painter.rotate(-j * 30);
        painter.drawText(txtRectangle, Qt::AlignCenter, QString::number(j));
        painter.restore();
    }
}
