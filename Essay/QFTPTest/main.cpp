#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QString>

QProcess *wget;
int       main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString ftpPath = "ftp://192.168.0.101/DATA20190823083302";
    //    QString ftpPath   = "ftp://192.168.0.101/DATA20190825103647";
    QString localPath = "E:/test";
    //    QString     logFile   = mConfigure.diskDrive + "/" + mConfigure.filePath + "/ZF9012.log";
    QStringList arguments;
    QStringList testArg;

    QDir downloadPath(localPath);
    if (!downloadPath.exists())
    {
        downloadPath.mkpath(localPath);
    }
    arguments << ftpPath
              << "--recursive"                       // "-r" 递归下载
              << "--directory-prefix=" + localPath   // "-P" 将文件保存到目录
              << "--no-directories"                  // "-nd" 不创建目录
              << "--no-host-directories"             // "-nH" 不创建主机目录
              << "--no-verbose"                      // "-nv" 简洁输出
              << "--no-clobber";                     // "-nc" 跳过重复的文件
    //              << "--ftp-user=user"
    //              << "--ftp-password=123456"
    //              << QString("--output-file=%1").arg(logFile);
    testArg = arguments;
    testArg << "--spider";
    wget = new QProcess(nullptr);
    //    QObject::connect(wget, &QProcess::readyReadStandardError, []() {
    //        qDebug() << "error:" << QString::fromLocal8Bit(wget->readAllStandardError());
    //    });
    //    QObject::connect(wget,
    //                     QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //                     [](int exitCode, QProcess::ExitStatus exitStatus) {
    //                         qDebug() << (exitStatus == QProcess::NormalExit ? "Normal" : "Crash");
    //                     });
    wget->start("wget.exe", testArg);
    wget->waitForFinished(-1);

    QStringList list = QString::fromLocal8Bit(wget->readAllStandardError()).split("[1]");
    int         size = list.size();
    qDebug() << "size:" << size;

    int       index = 0;
    QProcess *wget  = new QProcess(nullptr);
    QObject::connect(wget, &QProcess::readyReadStandardError, [&index, size]() {
        qDebug() << QString("down ZF9012:") + QString::number(++index) + "/" + QString::number(size);
    });

    QObject::connect(wget,
                     QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [size](int exitCode, QProcess::ExitStatus exitStatus) {
                         if (exitStatus == QProcess::NormalExit)
                         {
                             qDebug() << QString("down ZF9012:") + QString::number(size) + "/" + QString::number(size);
                         }
                     });
    wget->start("wget.exe", arguments);
    return a.exec();
}
