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

public slots:
protected:
    void customEvent(QEvent *event) override;
private slots:
    void slotOpenRealTimePreview(bool enable);
    // 连接/断开相机
    void slotConnectCamera();
    void slotDisconnectCamera();

    void slotCameraComboxChanged(const QString &sn);

    // 设置预览标定文件

    void slotSetSavePath();
    void slotSetCalibrationPath();
    void slotSetPulseOutput();
    void slotSetSerialPort();
    void slotSetBModelSerialPort();
    void slotSetWhiteBalance();
    void slotSetExposure();
    void slotSetConfig();

    void slotGetSerialPortInfo();
    void slotGetCameraInfo();
    void slotGetCameraState();
    void slotGetCollectionInfo();

    // 退出

    void slotStartGrabImage();
    void slotStopGrabImage();
    void slotStopWork();

    void slotQuitCamera();

private:
    bool checkFault();

    void grabImage();
    bool isConnect = false;

    Ui::Demo *                           ui;
    Teche::PhotoInfo                     mPhotoInfo;
    QMap<QString, Teche::CameraInfo>     mCameras;
    QMap<Teche::CameraWorkType, QString> mWorkType;
    QMap<Teche::PhotoQuality, QString>   mQualityType;
    QMap<Teche::parameterMode, QString>  mParameMode;
    QMap<Teche::parameterType, QString>  mParameterType;
};

#endif   // DEMO_H
