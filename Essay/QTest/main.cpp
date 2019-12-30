#include "PrintFormat.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QTime>
#include <QVector>
#include <fmt/format.h>
#include <map>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

//#pragma execution_character_set("utf-8")

struct A
{
    double a;
    double b;
};

template <typename T>
bool search_map(const std::map<T, A> &map, T v)
{
    auto search = map.find(v);
    if (search != map.end())
    {
        qDebug() << QString::fromUtf8(u8"ABCDEFC\u03BC");
        qDebug() << "a:" << search->second.a;
        qDebug() << "b:" << search->second.b;
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    double a = 123456.123456;
    printf("% 12.3lf\n", a);         // 右对齐
    printf("%-12.3lf\n", a);         // 左对齐
    printf("%0*.*lf\n", 12, 3, a);   // 右对齐前导0
    qDebug() << QString::asprintf("% 12.3lf\n", a);
    qDebug() << QString::asprintf("%-12.3lf\n", a);
    qDebug() << QString::asprintf("%0*.*lf\n", 12, 3, a);
    return 0;
}
