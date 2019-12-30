#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMap>
#include <QXmlStreamReader>

typedef std::function<void()> elementParse;
int                           main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString file = QFileDialog::getOpenFileName(nullptr, "xml", "C:/Users/zhaokai/Desktop");
    QFile   xmlFile(file);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "open xml file failed!";
        exit(1);
    }

    QMap<QString, elementParse> mElementsParse;
    QXmlStreamReader            mXmlReader;
    QXmlStreamAttributes        mAttributes;
    mElementsParse.insert("ret", [&]() {
        if (mAttributes.hasAttribute("cmd"))
            qDebug() << "cmd:" << mAttributes.value("cmd").toString();

        if (mAttributes.hasAttribute("err"))
            qDebug() << "err:" << mAttributes.value("err").toInt();

        if (mAttributes.hasAttribute("msg"))
            qDebug() << "msg:" << mAttributes.value("msg").toString();
    });

    mElementsParse.insert("scanresult", [&]() {
        while (!mXmlReader.isEndDocument() && (!mXmlReader.hasError()))
        {
            if (mXmlReader.readNextStartElement())
            {
                mAttributes  = mXmlReader.attributes();
                QString name = mXmlReader.name().toString();
                if (name == "scans")
                {
                    if (mAttributes.hasAttribute("path"))
                        qDebug() << "path:" << mAttributes.value("path").toString();
                }
                else if (name == "scan")
                {
                    if (mAttributes.hasAttribute("filesize"))
                    {
                        qDebug() << "filesize:" << mAttributes.value("filesize").toInt();
                        qDebug() << "file:" << mXmlReader.readElementText();
                    }
                }
            }
        }
    });

    mXmlReader.setDevice(&xmlFile);
    while ((!mXmlReader.isEndDocument()) && (!mXmlReader.hasError()))
    {
        if (mXmlReader.readNextStartElement())
        {
            mAttributes  = mXmlReader.attributes();
            QString name = mXmlReader.name().toString();
            qDebug() << "name:" << name;
            if (mElementsParse.contains(name))
                mElementsParse[name]();
        }
    }

    return 0;
}
