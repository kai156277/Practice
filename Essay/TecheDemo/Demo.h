#ifndef DEMO_H
#define DEMO_H

#include "ControlSdk.h"

#include <QMap>
#include <QWidget>

namespace Ui {
class Demo;
}

class Demo : public QWidget
{
    Q_OBJECT

public:
    explicit Demo(QWidget *parent = 0);
    ~Demo();

protected:
    void customEvent(QEvent *event) override;
private slots:
    void slotGetCameraInfo();
    void slotCameraComboxChanged(const QString &sn);
    void slotConnectCamera();
    void slotDisconnectCamera();
    void slotSetCalibrationPath();
    void slotOpenRealTimePreview(bool enable);
    bool slotCheckFault();

    void slotQuitCamera();

    void slotGetCameraState();
    void slotGPSInfo();
    void slotGrabImage(bool isGrab);
    void slotSetOutput();
    void slotSetSerialPort();
    void slotSetWhiteBalance();
    void slotSetExposure();
    void slotSetSavePath();

private:
    bool isConnect = false;

    Ui::Demo *                           ui;
    QMap<QString, Teche::CameraInfo>     mCameras;
    QMap<Teche::CameraWorkType, QString> mWorkType;
    QMap<Teche::PhotoQuality, QString>   mQualityType;
    QMap<Teche::parameterMode, QString>  mParameMode;
};

#endif   // DEMO_H
