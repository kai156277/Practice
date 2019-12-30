#include "ZF9012StatusDef.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <map>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::map<int, QString> items = {{1, "scanning time(sec)"},
                                    {2, "number of lines"},
                                    {3, "progress"},
                                    {4, "bytes written to file(Mb)"},
                                    {5, "pixel per second"},
                                    {6, "lines per second"},
                                    {8, "close(0) or far(1) + max range(mm)"},
                                    {10, "filterindex + filter frequenz(kHz)"},
                                    {12, "current horz position(deg)"},
                                    {14, "number of errors"},
                                    {15, "current step"},
                                    {20, "referenz range(mm)"},
                                    {21, "level x(deg)"},
                                    {22, "level y(deg)"}};

    QString openFile = QFileDialog::getOpenFileName(nullptr, "status Stream", "C:/Users/zhaokai/Desktop");
    //    QString openFile = "C:/Users/zhaokai/Desktop/statusstreamText";

    QFile statusFile(openFile);

    if (!statusFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error open status stream file:" << openFile;
        exit(1);
    }

    QDataStream read(&statusFile);

    char        rawData[514];
    char *      readPos = rawData;
    PackageItem itemValue;
    head        headValue;
    int         item_size = sizeof(PackageItem);   // 16
    int         head_size = sizeof(head);          // 16
                                                   //    while (read.readRawData(rawData, 513) != -1)
    while (!read.atEnd())
    {
        readPos = rawData;
        read.readRawData(rawData, 514);
        printf("------------------------ package ---------------------------\n");
        {
            memcpy(&headValue, readPos, head_size);
            readPos += head_size;
            printf("id: %#x\t", headValue.id);
            printf("seq: %d\t", headValue.seq);
            printf("package num: %d\t", headValue.packageNum);
            printf("num of packages: %d\n", headValue.numOfPackage);
        }

        for (int i = 0; i < 14; ++i)
        {
            memcpy(&itemValue, readPos, item_size);
            readPos += item_size;

            auto iter = items.find(itemValue.index);
            if (iter != items.end())
            {
                //                printf("index: %d \t type: %d \n", itemValue.index, itemValue.type);

                switch (itemValue.type)
                {
                case 1:
                    qDebug() << iter->second << ":" << itemValue.value._int32Value;
                    break;
                case 2:
                    qDebug() << iter->second << ":" << itemValue.value._doubleValue;
                    break;
                case 3:
                {
                    //                    qDebug() << "stirng size:" << itemValue.value._int32Value;
                    char *str = new char[itemValue.value._int32Value];
                    memcpy(str, readPos, itemValue.value._int32Value);
                    readPos += itemValue.value._int32Value;
                    qDebug() << "current step:" << str;
                    delete str;
                    break;
                }
                case 4:
                {
                    if (itemValue.index == 8)
                    {
                        qDebug() << (itemValue.value._groupValue.first ? "far" : "close")
                                 << "max range(mm)" << itemValue.value._groupValue.second;
                    }
                    else if (itemValue.index == 10)
                    {
                        printf("filterindex: %d\t", itemValue.value._groupValue.first);
                        printf("filter frequenz(kHz) : %d\n", itemValue.value._groupValue.second);
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
    }

    return 0;
}
