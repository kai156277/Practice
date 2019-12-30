#include "ConstVar.h"
#include "TestConstexptr.h"

#include <QDateTime>
#include <QDebug>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

enum class color
{
    red    = 1,
    yellow = 2
};

int main()
{
    TestConstexptr a;
    QString        timeStr  = "19961223 012346 UTC";
    QDateTime      dateTime = QDateTime::fromString(timeStr, "yyyyMMdd HHmmss UTC");
    dateTime.setTimeSpec(Qt::UTC);

    qDebug() << "Main:" << text;
    qDebug() << dateTime.toString();
    qDebug() << dateTime.toUTC().toString();
    qDebug() << dateTime.toLocalTime().toString();
    qDebug() << (((int) color::red == 2) ? "True" : "False");
    qDebug() << ((color::red == (color) 2) ? "True" : "False");

    QString num = "   .1234  1";
    qDebug() << "num:" << num.toDouble();
    return 0;
}
