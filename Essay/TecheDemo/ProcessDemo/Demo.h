#ifndef DEMO_H
#define DEMO_H

#include <QDateTime>
#include <QWidget>
#include <SwitchDll.h>

namespace Ui {
class Demo;
}

class Demo : public QWidget
{
    Q_OBJECT

public:
    explicit Demo(QWidget *parent = 0);
    ~Demo();
signals:
    void saveImage(int count, Teche::ConvertState state);
public slots:
    void slotSetInputFolder();
    void slotSetCalibrationFile();
    void slotSetSerialPortFile();

    void slotStartProcessTeFile();
    void slotStopProcessTeFile();

private slots:
    void slotSaveImage(int count, Teche::ConvertState state);
    void slotCreateTimeFile();

private:
    void      createTimeFile(const QString &path);
    Ui::Demo *ui;

    QDateTime mBegin;
    QDateTime mEnd;
    int       mCount = 1;

    bool                      isStop             = false;
    Teche::ConvertFile *      mConvertFile       = nullptr;
    Teche::ConvertSwitchFile *mConvertSwitchFile = nullptr;
};

#endif   // DEMO_H
