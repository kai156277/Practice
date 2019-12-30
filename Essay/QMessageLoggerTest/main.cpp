#include <QCoreApplication>
#include <QDebug>
#include <QMessageLogContext>
#include <QMessageLogger>
#include <iostream>
#include <stdio.h>

//#pragma execution_character_set("utf-8")

std::FILE *fp;
void       myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    switch (type)
    {
    case QtDebugMsg:
        fprintf(fp,
                "Debug: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr,
                "Info: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr,
                "Warning: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr,
                "Critical: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr,
                "Fatal: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        abort();
    }
}

int main(int argc, char *argv[])
{
    fp = fopen("test.txt", "w");
    if (!fp)
    {
        std::cout << "open error!" << std::endl;
    }
    QCoreApplication a(argc, argv);
    //    qInstallMessageHandler(myMessageOutput);

    QString     test("来自 QString 测试文本");
    std::string test1("来自 std::string 测试文本");
    std::string test2(test.toLocal8Bit().data());

    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale(""));
    std::wcout << L"来自 std::wstring 测试文本" << std::endl;

    char tmp[256];
    std::cout << "test1 size:" << test1.size() << std::endl;
    strcpy(tmp, test1.c_str());
    std::cout << test.toLocal8Bit().data() << std::endl;
    std::cout << "size: " << test.size() << std::endl;
    std::cout << test1 << std::endl;
    std::cout << test2 << std::endl;
    std::cout << tmp << std::endl;
    std::cout << std::string(tmp) << std::endl;

    qDebug() << "---------------";
    qDebug() << QStringLiteral("来自 QStringLiteral 测试文本");
    qDebug() << test;
    qDebug() << QString::fromUtf8(test1.data());
    qDebug() << QString(tmp);
    qDebug() << QString(test1.c_str());
    qDebug() << tmp;

    QString b = "bababaa";
    qDebug() << b;
    qDebug("%f", 12.333);
    return 0;
}
