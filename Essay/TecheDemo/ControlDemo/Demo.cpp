#include "Demo.h"

#include "ui_Demo.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#include "ExposureSettingDialog.h"
#include "PulseOutputSettingDialog.h"
#include "WhiteBalanceSettingDialog.h"

#include <io.h>
#include <iostream>
#include <string>

// opencv
#include <ControlSdk.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>

#pragma warning(disable : 4996)
#pragma execution_character_set("utf-8")

using namespace Teche;

Demo *       smDemo = nullptr;
QEvent::Type sType;

class PostImageEvent : public QEvent
{
public:
    PostImageEvent(Type type, const QImage &image)
        : QEvent(type)
        , mImage(image)
    {
    }
    QImage mImage;
};

Demo::Demo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Demo)
{
    ui->setupUi(this);
    smDemo = this;
    sType  = (QEvent::Type) QEvent::registerEventType();

    mWorkType.insert(InternalRigger, "内部触发");
    mWorkType.insert(ExternalRigger, "外部触发");
    mWorkType.insert(NoRigger, "未设置触发");
    mWorkType.insert(InternalRiggerOnePush, "内部触发 单点触发");

    ui->mWorkTypeComboBox->addItem(mWorkType.value(InternalRigger), InternalRigger);
    ui->mWorkTypeComboBox->addItem(mWorkType.value(ExternalRigger), ExternalRigger);
    ui->mWorkTypeComboBox->addItem(mWorkType.value(InternalRiggerOnePush), InternalRiggerOnePush);
    ui->mWorkTypeComboBox->addItem(mWorkType.value(NoRigger), NoRigger);

    mQualityType.insert(Standard, "标准");
    mQualityType.insert(Good, "好");
    mQualityType.insert(Best, "最佳");

    ui->mQualityComboBox->addItem(mQualityType.value(Standard), Standard);
    ui->mQualityComboBox->addItem(mQualityType.value(Good), Good);
    ui->mQualityComboBox->addItem(mQualityType.value(Best), Best);

    mParameMode.insert(Follow, "跟随");
    mParameMode.insert(Independent, "独立");

    mParameterType.insert(Auto, "自动");
    mParameterType.insert(Manual, "手动");
    mParameterType.insert(ShutterPriority, "快门优先");
    mParameterType.insert(ISOPriority, "ISO优先");
    QSettings config;

    ui->mCameraInfoLabel->setTextFormat(Qt::RichText);
    ui->mCameraInfoLabel->setAlignment(Qt::AlignCenter);
    ui->mCalibrationPathLineEdit->setReadOnly(true);

    if (!config.contains("calibrationPath") || config.value("calibrationPath").isNull())
    {
        slotSetCalibrationPath();
    }
    else
    {
        QString calibration = config.value("calibrationPath", "").toString();
        ui->mCalibrationPathLineEdit->setText(calibration);
    }

    ui->mSavePathLineEdit->setReadOnly(true);
    if (!config.contains("savePath") || config.value("savePath").isNull())
    {
        slotSetSavePath();
    }
    else
    {
        QString savePath = config.value("savePath", "").toString();
        ui->mSavePathLineEdit->setText(savePath);
    }
    connect(ui->mUpdateCameraPushButton, &QPushButton::clicked, this, &Demo::slotGetCameraInfo);

    connect(ui->mCamerasComboBox, &QComboBox::currentTextChanged, this, &Demo::slotCameraComboxChanged);
    connect(ui->mConnectPushButton, &QPushButton::clicked, this, &Demo::slotConnectCamera);
    connect(ui->mDisconnectPushButton, &QPushButton::clicked, this, &Demo::slotDisconnectCamera);
    connect(ui->mCameraStatePushButton, &QPushButton::clicked, this, &Demo::slotGetCameraState);

    connect(ui->mConfigPushButton, &QPushButton::clicked, this, &Demo::slotSetConfig);
    connect(ui->mStartPushButton, &QPushButton::clicked, this, &Demo::slotStartGrabImage);
    connect(ui->mStopPushButton, &QPushButton::clicked, this, &Demo::slotStopGrabImage);
    connect(ui->mSavePathPushButton, &QPushButton::clicked, this, &Demo::slotSetSavePath);
    connect(ui->mCollectionInfoPushButton, &QPushButton::clicked, this, &Demo::slotGetCollectionInfo);

    connect(ui->mPreviewPushButton, &QPushButton::toggled, this, &Demo::slotOpenRealTimePreview);
    connect(ui->mCalibrationPathPushButton, &QPushButton::clicked, this, &Demo::slotSetCalibrationPath);

    connect(ui->mPulseOutputPushButton, &QPushButton::clicked, this, &Demo::slotSetPulseOutput);
    connect(ui->mSerialPortPushButton, &QPushButton::clicked, this, &Demo::slotSetSerialPort);
    connect(ui->mBSeralProtPushButton, &QPushButton::clicked, this, &Demo::slotSetBModelSerialPort);
    connect(ui->mExposurePushButton, &QPushButton::clicked, this, &Demo::slotSetExposure);
    connect(ui->mWhiteBalancePushButton, &QPushButton::clicked, this, &Demo::slotSetWhiteBalance);

    connect(ui->mSerialPortInfoPushButton, &QPushButton::clicked, this, &Demo::slotGetSerialPortInfo);
    connect(ui->mStopWorkPushButton, &QPushButton::clicked, this, &Demo::slotStopWork);
}

Demo::~Demo()
{
    slotQuitCamera();
    delete ui;
}

void Demo::customEvent(QEvent *event)
{
    if (event->type() == sType)
    {
        PostImageEvent *imageEvent = dynamic_cast<PostImageEvent *>(event);
        if (imageEvent)
            ui->mDisplayImageLabel->setPixmap(QPixmap::fromImage(imageEvent->mImage));
        else
            qDebug() << "无法转换为图像";
    }
}

void Demo::slotGetCameraInfo()
{
    std::vector<Teche::CameraInfo> cameras = Teche::ControlSdk::QueryCamera();
    for (int i = 0; i < cameras.size(); ++i)
    {
        Teche::CameraInfo camera = cameras.at(i);
        QString           sn     = QString::fromStdString(camera.sn);
        mCameras.insert(sn, camera);
        ui->mCamerasComboBox->addItem(sn);

        QString info;
        info.append("可以使用的相机");
        info.append(QString("sn: %1 ").arg(QString::fromStdString(camera.sn)));
        info.append(QString("model: %1 ").arg(QString::fromStdString(camera.model)));
        info.append(QString("mac: %1 ").arg(QString::fromStdString(camera.macAddr)));
        info.append(QString("ip: %1 ").arg(QString::fromStdString(camera.ip)));
        info.append(QString("conectClientIp: %1 ").arg(QString::fromStdString(camera.connectClientIp)));
        info.append(QString("version: %1").arg(QString::fromStdString(camera.version)));

        qDebug() << info;
    }
    qDebug() << "camers:" << cameras.size();
}

void Demo::slotStopWork()
{
    bool ret = Teche::ControlSdk::SetOperation(Teche::Stop);
    if (ret)
    {
        qDebug() << "停止相机工作成功！";
    }
    else
    {
        qDebug() << "停止相机工作失败！";
    }
}

void Demo::slotCameraComboxChanged(const QString &sn)
{
    if (!mCameras.contains(sn))
    {
        qWarning() << "未找到存储的相机信息";
        return;
    }
    Teche::CameraInfo camera = mCameras.value(sn);

    QString info;
    info.append(R"(<p style="color: green">当前使用的相机: )");
    info.append(QDateTime::currentDateTime().toString() + "</p>");
    info.append(QString("<p> sn: %1 ").arg(QString::fromStdString(camera.sn)));
    info.append(QString("model: %1 ").arg(QString::fromStdString(camera.model)));
    info.append(QString("mac: %1 ").arg(QString::fromStdString(camera.macAddr)));
    info.append(QString("ip: %1 ").arg(QString::fromStdString(camera.ip)));
    info.append(QString("conectClientIp: %1 ").arg(QString::fromStdString(camera.connectClientIp)));
    info.append(QString("version: %1 </p>").arg(QString::fromStdString(camera.version)));

    ui->mCameraInfoLabel->setText(info);
}

void Demo::slotConnectCamera()
{
    QString sn = ui->mCamerasComboBox->currentText();
    if (!mCameras.contains(sn))
    {
        qWarning() << "未找到存储的相机信息";
        return;
    }
    Teche::CameraInfo camera = mCameras.value(sn);

    bool ret = Teche::ControlSdk::ConnectCamera(&camera);
    if (ret && checkFault())
    {
        qDebug() << "连接相机成功";
        ui->mConnectPushButton->setEnabled(false);
        ui->mDisconnectPushButton->setEnabled(true);
        isConnect = true;
    }
    else
    {
        qWarning() << "连接相机失败";
        ui->mConnectPushButton->setEnabled(true);
        ui->mDisconnectPushButton->setEnabled(false);
    }
}

void Demo::slotDisconnectCamera()
{
    bool ret = Teche::ControlSdk::CloseCamera(false);
    if (ret)
    {
        qDebug() << "断开连接成功";
        ui->mConnectPushButton->setEnabled(true);
        ui->mDisconnectPushButton->setEnabled(false);
        isConnect = false;
    }
    else
    {
        qWarning() << "断开连接失败";
        ui->mConnectPushButton->setEnabled(false);
        ui->mDisconnectPushButton->setEnabled(true);
    }
}

void Demo::slotSetCalibrationPath()
{
    QSettings config;
    QString   defFile = config.value("calibrationPath", "").toString();
    QString   calibration =
        QFileDialog::getOpenFileName(this, "预览标定文件", defFile, "标定文件 (*.pts)");
    if (calibration.isEmpty())
        return;
    config.setValue("calibrationPath", calibration);

    ui->mCalibrationPathLineEdit->setText(calibration);
}

void Demo::slotOpenRealTimePreview(bool enable)
{
    Teche::ControlSdk::RealTimeImageCallBackFunc tmp = [](unsigned char *      bgrChar,
                                                          Teche::realTimeState state) {
        if (state == Teche::realTimeState::Preview)
        {
            cv::Mat img = cv::Mat(540, 1080, CV_8UC3, (void *) bgrChar);

            cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
            QImage image((const unsigned char *) img.data,
                         img.cols,
                         img.rows,
                         img.step,
                         QImage::Format_RGB888);

            PostImageEvent *imageEvent = new PostImageEvent(sType, image);
            QApplication::postEvent(smDemo, imageEvent);
        }
        else if (state == Teche::realTimeState::PreviewEnd)
        {
            qDebug() << "预览完毕";
        }
        else
        {
            qDebug() << "执行方法错了！";
        }
    };
    qDebug() << (enable ? "True" : "False");
    if (enable)
    {
        QString   calibration = ui->mCalibrationPathLineEdit->text();
        QFileInfo info(calibration);
        if (info.exists() && info.suffix() == "pts")
        {
            qDebug() << calibration;
            Teche::ControlSdk::GetRealTimeImage(tmp,
                                                std::string(calibration.toLocal8Bit().constData()));
            ui->mPreviewPushButton->setText("预览中");
        }
        else
        {
            qWarning() << "未输入有效的预览标定文件";
        }
    }
    else
    {
        bool ret = Teche::ControlSdk::StopRealTimeImage();
        if (ret)
        {
            qDebug() << "断开预览流成功";
            ui->mPreviewPushButton->setText("结束预览");
            ui->mDisplayImageLabel->setText("实时预览相片");
        }
        else
        {
            qWarning() << "断开预览流失败";
        }
    }
}

bool Demo::checkFault()
{
    Teche::faultType ft;
    QString          html;

    bool ret = Teche::ControlSdk::GetFault(&ft);

    if (ret)
    {
        if (ft == Teche::Normal)
        {
            qDebug() << "检测相机无故障，启动成功";
            return true;
        }
        else if (ft == Teche::CameraInit)
        {
            qWarning() << "镜头在初始化中";
        }
        else if (ft == Teche::CameraFault)
        {
            qWarning() << "镜头初始化失败！请重启相机再次连接";
            slotDisconnectCamera();
        }
        else if (ft == Teche::InternetSpeedFault)
        {
            qWarning() << "带宽不够！请检查网络 ";
        }
        else
        {
        }
    }
    else
    {
        qWarning() << "查询相机故障失败，请检查相机是否正确开机！";
    }
    return false;
}

void Demo::slotQuitCamera()
{
    if (isConnect)
        Teche::ControlSdk::CloseCamera(false);

    QString sn = ui->mCamerasComboBox->currentText();
    if (!mCameras.contains(sn))
    {
        qWarning() << "未找到存储的相机信息";
        return;
    }
    Teche::CameraInfo camera = mCameras.value(sn);

    bool ret = Teche::ControlSdk::ConnectCamera(&camera);
    if (ret)
    {
        ret = Teche::ControlSdk::CloseCamera(true);
        qDebug() << (ret ? "退出相机成功" : "退出相机失败");
    }
}

void Demo::slotSetConfig()
{
    mPhotoInfo.fps          = ui->mFpsSpinBox->value();
    mPhotoInfo.photoQuality = (Teche::PhotoQuality) ui->mQualityComboBox->currentData().toInt();
    mPhotoInfo.workType     = (Teche::CameraWorkType) ui->mWorkTypeComboBox->currentData().toInt();
    mPhotoInfo.width        = 4000;
    mPhotoInfo.height       = 3000;

    qDebug() << "画质：" << ui->mQualityComboBox->currentText();
    qDebug() << "工作模式：" << ui->mWorkTypeComboBox->currentText();
    qDebug() << "FPS：" << mPhotoInfo.fps;
    bool ret = Teche::ControlSdk::SetCameraModelPhoto(&mPhotoInfo, true);
    if (ret)
    {
        qDebug() << "设置相机工作模式成功！";
    }
    else
    {
        qDebug() << "设置相机工作失败！请查询相机工作状态。";
    }
}

void Demo::slotGetCameraState()
{
    Teche::CameraState state;

    bool ret = Teche::ControlSdk::GetCameraState(&state);
    if (ret)
    {
        if (state == Teche::Open)
        {
            //读取当前相机工作状态
            qDebug() << "------------------------ 相机已连接 ----------------------";
            Teche::CurrentCameraInfo currentCameraInfo;

            ret = Teche::ControlSdk::GetCurrentCameraState(&currentCameraInfo);
            if (ret)   //获取状态成功
            {
                qDebug() << "触发类型：" << mWorkType.value(currentCameraInfo.workType);
                qDebug() << "相片质量：" << mQualityType.value(currentCameraInfo.photoQuality);
                qDebug() << "分辨率：";
                qDebug() << QString("宽：%1 高: %2").arg(currentCameraInfo.width, currentCameraInfo.height);

                qDebug() << "白平衡：";
                qDebug() << "模式：" + mParameMode.value(currentCameraInfo.titanWbInfo.mode);
                if (currentCameraInfo.titanWbInfo.mode == Teche::Follow)
                {
                    int wbMaster = currentCameraInfo.titanWbInfo.cameraList[0].cameraIndex;
                    qDebug() << QString("主镜头：%1 类型：自动").arg(wbMaster);
                }
                else
                {
                    for (int i = 0; i < currentCameraInfo.titanWbInfo.cameraList.size(); i++)
                    {

                        CameraWbInfo camera = currentCameraInfo.titanWbInfo.cameraList[i];
                        qDebug() << QString("镜头索引：%1 类型：%2 r: %3 b: %4 gr: %5 gb: %6")
                                        .arg(i)
                                        .arg((camera.type == Teche::WbAuto) ? "自动" : "手动")
                                        .arg(camera.r)
                                        .arg(camera.b)
                                        .arg(camera.gr)
                                        .arg(camera.gb);
                    }
                }

                //曝光模式
                qDebug() << "曝光：";
                qDebug() << "模式：" + mParameMode.value(currentCameraInfo.titanExposureInfo.mode);
                if (currentCameraInfo.titanExposureInfo.mode == Teche::Follow)
                {
                    CameraExposureInfo camera = currentCameraInfo.titanExposureInfo.cameraList[0];

                    qDebug() << QString("\t主镜头：%1 类型：%2 补偿: %3 ISO: %4 曝光时间: %5")
                                    .arg(camera.cameraIndex)
                                    .arg(mParameterType.value(camera.type))
                                    .arg(camera.compensation)
                                    .arg(camera.iso)
                                    .arg(camera.expo);
                }
                else
                {
                    for (int i = 0; i < currentCameraInfo.titanExposureInfo.cameraList.size(); i++)
                    {
                        CameraExposureInfo camera =
                            currentCameraInfo.titanExposureInfo.cameraList[i];
                        qDebug() << QString("\t镜头索引：%1 类型：%2 补偿: %3 ISO: %4 曝光时间: %5")
                                        .arg(camera.cameraIndex)
                                        .arg(mParameterType.value(camera.type))
                                        .arg(camera.compensation)
                                        .arg(camera.iso)
                                        .arg(camera.expo);
                    }
                }

                if (currentCameraInfo.outputPulseEnable)   //开启脉冲输出
                {
                    QString pulsePolarity = "";
                    if (currentCameraInfo.pulsePolarity == 0)
                    {
                        pulsePolarity = "低";
                    }
                    else
                    {
                        pulsePolarity = "高";
                    }

                    qDebug() << "外部曝光：已开启";
                    qDebug() << QString("脉冲宽度：%1(ms) 脉冲延迟：%2(ms) 脉冲极性：%3")
                                    .arg(currentCameraInfo.pulseWidth)
                                    .arg(currentCameraInfo.pulseDelay)
                                    .arg(pulsePolarity);
                }
                else   //外部曝光未开启
                {
                    qDebug() << "外部曝光：未开启";
                }

                qDebug() << "相机状态：" << (currentCameraInfo.isWork ? "工作中" : "空闲");
            }
            else
            {
                qDebug() << "获取当前工作状态失败！请联系技术人员。";
            }
        }
        else
        {
            qDebug() << "相机已经断开连接！请重新进行连接!";
        }
    }
    else
    {
        qWarning() << "相机已经断开连接!请重新进行连接。";
    }
}

void Demo::slotGetSerialPortInfo()
{
    bool                 retState;
    Teche::MeasuringInfo curMeasuringInfo;

    retState = Teche::ControlSdk::GetCameraMeasuringInfo(&curMeasuringInfo);

    if (retState)
    {
        qDebug() << "查询串口信息!";
        if (curMeasuringInfo.serialMode == Teche::GPS)
        {

            qDebug() << "相机工作在GPS模式:" << (curMeasuringInfo.gpsInfo.valid ? "有效" : "无效");
            qDebug() << QString("卫星数：%1 经度：%2 维度：%3  高度：%4 卫星时间：%5")
                            .arg(curMeasuringInfo.gpsInfo.satellites)
                            .arg(curMeasuringInfo.gpsInfo.longitude)
                            .arg(curMeasuringInfo.gpsInfo.latitude)
                            .arg(curMeasuringInfo.gpsInfo.altitude)
                            .arg(QString::fromStdString(curMeasuringInfo.gpsInfo.datetime));
        }
        else
        {
            qDebug() << "相机工作在PPS模式" << (curMeasuringInfo.ppsInfo.valid ? "有效" : "无效");
            qDebug() << "当前PPS时间：" + QString::fromStdString(curMeasuringInfo.ppsInfo.datetime);
            qDebug() << "上一次同步的PPS时间：" + QString::fromStdString(curMeasuringInfo.ppsInfo.refDatetime);
            qDebug() << "上一次PPS同步的时间戳：" << curMeasuringInfo.ppsInfo.refTimestamp;
        }
        qDebug() << "陀螺仪信息：";
        qDebug() << QString("滚转: %1 俯仰：%2 偏航：%3")
                        .arg(curMeasuringInfo.gyroInfo.eulerR)
                        .arg(curMeasuringInfo.gyroInfo.eulerP)
                        .arg(curMeasuringInfo.gyroInfo.eulerH);

        qDebug() << QString("系统时间：%1 对应时间差：%2")
                        .arg(QString::fromStdString(curMeasuringInfo.datetime))
                        .arg(curMeasuringInfo.timestamp);
    }
    else
    {
        qDebug() << "查询失败!";
    }
}

void Demo::grabImage()
{
    qDebug() << "----------------- 采集相片 -----------------";
    QString savePath = ui->mSavePathLineEdit->text();
    QDir    saveDir(ui->mSavePathLineEdit->text());
    QString cal = QFileInfo(ui->mCalibrationPathLineEdit->text()).baseName();

    QMap<Teche::PhotoQuality, QString> qualityMap;
    qualityMap.insert(Standard, "Standard");
    qualityMap.insert(Good, "Good");
    qualityMap.insert(Best, "Best");

    QMap<Teche::CameraWorkType, QString> worktypeMap;
    worktypeMap.insert(InternalRigger, "In");
    worktypeMap.insert(ExternalRigger, "Out");
    worktypeMap.insert(NoRigger, "No");
    worktypeMap.insert(InternalRiggerOnePush, "OnePush");

    QString subDir = QString("Te%1_%2fps_%3x%4_%5_%6_%7")
                         .arg(cal)
                         .arg(mPhotoInfo.fps)
                         .arg(mPhotoInfo.height)
                         .arg(mPhotoInfo.width)
                         .arg(qualityMap.value(mPhotoInfo.photoQuality))
                         .arg(worktypeMap.value(mPhotoInfo.workType))
                         .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    if (saveDir.mkdir(subDir))
        savePath = ui->mSavePathLineEdit->text() + "/" + subDir;

    if (ui->mSaveSerialPortFileCheckBox->isChecked())
    {
        QString     serialPortFile = savePath + "/" + subDir + ".txt";
        std::string temp(serialPortFile.toLocal8Bit().constData());
        qDebug() << "serialPortFile:" << serialPortFile;
        // 1000是毫秒，表明1秒采集一次 不易过快 占用通讯通道 建议最快400
        std::thread doThreadSavePanoMat(Teche::ControlSdk::StartMeasuring, temp, 400);
        doThreadSavePanoMat.detach();
    }

    std::string inputStr(savePath.toLocal8Bit().constData());
    qDebug() << "savePath:" << savePath;
    bool ret = Teche::ControlSdk::SetOperation(Teche::Start);
    if (ret)
    {
        Sleep(500);
        ////初始化保存
        // Teche::ControlSdk::InitSaveImageInfo(false);
        // Sleep(1000);

        auto tmp = [](int saveCount) { qDebug() << "当前保存完照片组数：" << saveCount; };
        //初始化保存方法
        Teche::ControlSdk::SaveImageInfo(const_cast<char *>(inputStr.c_str()), tmp);

        Sleep(3500);

        //TODO: 完全失败处理
        //开始启动。前方休眠好后直接能启动，如果没有给前面方法初始化时间，那么这个方法就要运行几次等初始化成功后才能开始拍摄
        qDebug() << "工作类型：" << mWorkType.value(mPhotoInfo.workType);
        while (!Teche::ControlSdk::StartSaveTe(mPhotoInfo.workType))
        {
            qDebug() << "相机未准备成功";
            Sleep(1000);
        }
        qDebug() << "开始拍摄";
    }
    else
    {
        qDebug() << "开始失败！请查询相机工作状态。";
    }
}

void Demo::slotSetPulseOutput()
{
    PulseOutputSettingDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        bool retState = Teche::ControlSdk::SetOutputPulse(dialog.pulseInfo);

        if (retState)
        {
            qDebug() << "设置脉冲输出功能成功！";
        }
        else
        {
            qDebug() << "设置脉冲输出失败！请重试或联系相关技术人员！";
        }
    }
}

void Demo::slotSetSerialPort()
{
    QStringList measurings;
    bool        ok = false;
    measurings << "GPS"
               << "PPS";
    bool    measuringResult = false;
    QString value           = QInputDialog::getItem(this, "输入串口设置:", "串口", measurings, 0, false, &ok);
    if (ok && !value.isEmpty())
    {
        if (value == measurings[0])
        {
            measuringResult = Teche::ControlSdk::SetCameraGPSModel();
        }
        else if (value == measurings[1])
        {
            QStringList baudRates;
            baudRates << "115200"
                      << "57600"
                      << "38400"
                      << "19200"
                      << "9600";
            QString baudRate =
                QInputDialog::getItem(this, "输入波特率:", "波特率:", baudRates, 0, false, &ok);
            if (ok && !baudRate.isEmpty())
            {
                int baud        = baudRate.toInt();
                measuringResult = Teche::ControlSdk::SetCameraPPSModel(0, baud);
            }
        }
        else
        {
            qDebug() << "无效的输入";
        }

        if (measuringResult)
        {
            qDebug() << "串口信息设置成功！";
        }
        else
        {
            qDebug() << "串口信息设置失败！";
        }
    }
}
void Demo::slotSetBModelSerialPort()
{
    bool ret = Teche::ControlSdk::SetCameraPPSModel(0, 9600);
    if (ret)
    {
        ui->mSaveSerialPortFileCheckBox->setChecked(true);
        qDebug() << "串口信息设置成功！";
    }
    else
    {
        qDebug() << "串口信息设置失败！";
    }
}

void Demo::slotSetWhiteBalance()
{
    WhiteBalanceSettingDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Teche::TitansWbInfo wbInfo = dialog.WbInfo;

        qDebug() << "相机白平衡设置: " << ((wbInfo.mode == Teche::Follow) ? "跟随" : "独立");
        for (int i = 0; i < wbInfo.cameraList.size(); i++)
        {
            qDebug() << "Index:" << wbInfo.cameraList[i].cameraIndex;
            qDebug() << "Type" << ((wbInfo.cameraList[i].type == Teche::WbAuto) ? "自动" : "手动");
            qDebug() << "b" << wbInfo.cameraList[i].b;
            qDebug() << "r" << wbInfo.cameraList[i].r;
            qDebug() << "gb" << wbInfo.cameraList[i].gb;
            qDebug() << "gr" << wbInfo.cameraList[i].gr;
        }

        bool ret = Teche::ControlSdk::SetWb(wbInfo);
        if (ret)
            qDebug() << "设置白平衡成功";
        else
            qDebug() << "设置白平衡失败";
    }
}

void Demo::slotSetExposure()
{
    ExposureSettingDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Teche::TitansExposureInfo expInfo = dialog.exposure;
        qDebug() << "相机曝光设置：" << ((expInfo.mode == Teche::Follow) ? "跟随" : "独立");
        for (int i = 0; i < expInfo.cameraList.size(); i++)
        {
            qDebug() << "Index:" << expInfo.cameraList[i].cameraIndex;
            qDebug() << "Type:" << mParameterType.value(expInfo.cameraList[i].type);
            qDebug() << "compensation:" << expInfo.cameraList[i].compensation;
            qDebug() << "expo:" << expInfo.cameraList[i].expo;
            qDebug() << "iso:" << expInfo.cameraList[i].iso;
        }
        bool ret = Teche::ControlSdk::SetExposure(expInfo);
        if (ret)
            qDebug() << "设置曝光成功";
        else
            qDebug() << "设置曝光失败";
    }
}

void Demo::slotSetSavePath()
{
    QSettings config;
    QString   defFile  = config.value("savePath", "").toString();
    QString   savepath = QFileDialog::getExistingDirectory(this,
                                                         "文件保存路径",
                                                         defFile,
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (savepath.isEmpty())
        return;

    config.setValue("savePath", savepath);
    ui->mSavePathLineEdit->setText(savepath);
}

void Demo::slotGetCollectionInfo()
{
    Teche::CollectionStatusInfo info;

    bool collState = Teche::ControlSdk::GetCameraCollectionStatusInfo(&info);

    if (collState)
    {
        qDebug() << "采集模式：" << mWorkType.value(info.workType);
        qDebug() << "采集时长：" << info.collectionTime;
        qDebug() << "采集丢帧数：" << info.collectionLostFrames;
        qDebug() << "采集数量：" << info.collectionFrames;
        qDebug() << "采集频率：" << info.collectionFps;
    }
    else
    {
        qDebug() << "查询采集状态报错！";
    }
}

void Demo::slotStartGrabImage()
{
    slotSetConfig();
    grabImage();
}

void Demo::slotStopGrabImage()
{
    bool retState = Teche::ControlSdk::SetOperation(Teche::Stop);

    if (retState)
    {
        Sleep(1000);

        retState = Teche::ControlSdk::StopGetImageInfo();

        if (retState)
        {
            Teche::ControlSdk::StopMeasuring();

            //            ui->mStopPushButton->setEnabled(false);
            //            ui->mStartPushButton->setEnabled(true);
            qDebug() << "停止相机拍照成功！";
        }
        else
        {
            qDebug() << "停止程序采集失败！";
        }
    }
    else
    {
        qDebug() << "停止相机拍照失败！";
    }
}
