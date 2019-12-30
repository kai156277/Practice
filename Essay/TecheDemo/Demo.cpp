#include "Demo.h"

#include "ui_Demo.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#include "io.h"

#include <iostream>
#include <string>

// opencv
#include "ControlSdk.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#pragma warning(disable : 4996)
#pragma execution_character_set("utf-8")

using namespace Teche;

Demo *       smDemo = nullptr;
QEvent::Type sType;

#if 0
void RealTimeImageCallBackFunc(unsigned char *bgrChar, Teche::realTimeState switchState)
{
    if (switchState == Teche::realTimeState::Preview)
    {
        //调用图片调试分辨率
        cv::Mat img = cv::Mat(540, 1080, CV_8UC3, (void *) bgrChar);

        //cv::namedWindow("camera1", CV_WINDOW_NORMAL);//自适应
        cv::namedWindow("camera1");   //原始大小
        cv::imshow("camera1", img);
        cv::waitKey(1);
        //std::cout << "预览中。。。。。。" << std::endl;
    }
    else if (switchState == Teche::realTimeState::PreviewEnd)
    {
        std::cout << "预览完毕" << std::endl;
    }
    else
    {
        std::cout << "执行方法报错了！！！！" << std::endl;
    }
}
#endif

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
    mWorkType.insert(InternalRigger, "InternalRigger");
    mWorkType.insert(ExternalRigger, "ExternalRigger");
    mWorkType.insert(NoRigger, "NoRigger");
    mWorkType.insert(InternalRiggerOnePush, "InternalRiggerOnePush");

    mQualityType.insert(Standard, "Standard");
    mQualityType.insert(Good, "Good");
    mQualityType.insert(Best, "Best");

    mParameMode.insert(Follow, "Follow");
    mParameMode.insert(Independent, "Independent");

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

    connect(ui->mCamerasComboBox, &QComboBox::currentTextChanged, this, &Demo::slotCameraComboxChanged);
    connect(ui->mPreviewPushButton, &QPushButton::toggled, this, &Demo::slotOpenRealTimePreview);
    connect(ui->mGrabPushButton, &QPushButton::toggled, this, &Demo::slotGrabImage);
    connect(ui->mUpdateCameraPushButton, &QPushButton::clicked, this, &Demo::slotGetCameraInfo);
    connect(ui->mConnectPushButton, &QPushButton::clicked, this, &Demo::slotConnectCamera);
    connect(ui->mDisconnectPushButton, &QPushButton::clicked, this, &Demo::slotDisconnectCamera);
    connect(ui->mCalibrationPathPushButton, &QPushButton::clicked, this, &Demo::slotSetCalibrationPath);
    connect(ui->mCameraStatePushButton, &QPushButton::clicked, this, &Demo::slotGetCameraState);
    connect(ui->mGPSInfoPushButton, &QPushButton::clicked, this, &Demo::slotGPSInfo);
    connect(ui->mPulseOutputPushButton, &QPushButton::clicked, this, &Demo::slotSetOutput);
    connect(ui->mSerialPortPushButton, &QPushButton::clicked, this, &Demo::slotSetSerialPort);
    connect(ui->mExposurePushButton, &QPushButton::clicked, this, &Demo::slotSetExposure);
    connect(ui->mSavePathPushButton, &QPushButton::clicked, this, &Demo::slotSetSavePath);
}

Demo::~Demo()
{
    //    slotDisconnectCamera();
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
    if (ret && slotCheckFault())
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
    QString   defFile     = config.value("calibrationPath", "").toString();
    QString   calibration = QFileDialog::getOpenFileName(this, "预览标定文件", defFile, "标定文件 (*.pts)");
    if (calibration.isEmpty())
        return;
    config.setValue("calibrationPath", calibration);

    ui->mCalibrationPathLineEdit->setText(calibration);
}

void Demo::slotOpenRealTimePreview(bool enable)
{
    Teche::ControlSdk::RealTimeImageCallBackFunc tmp =
        [](unsigned char *bgrChar, Teche::realTimeState state) {
            if (state == Teche::realTimeState::Preview)
            {
                cv::Mat img = cv::Mat(540, 1080, CV_8UC3, (void *) bgrChar);
                //                cv::namedWindow("camera1");
                //                cv::imshow("camera1", img);
                //                cv::waitKey(1);

                cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
                QImage image((const unsigned char *) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

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
            qDebug() << calibration.toLocal8Bit().constData();
            Teche::ControlSdk::GetRealTimeImage(tmp, std::string(calibration.toLocal8Bit().constData()));
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

bool Demo::slotCheckFault()
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

void Demo::slotGetCameraState()
{
    Teche::CameraState state;
    bool               ret = Teche::ControlSdk::GetCameraState(&state);
    QString            html;
    if (ret)
    {
        html.append(R"(<p style="color: green">相机状态： )");
        html.append(QDateTime::currentDateTime().toString() + "</p>");
        if (state == Teche::Open)
        {
            //读取当前相机工作状态
            Teche::CurrentCameraInfo currentCameraInfo;
            bool                     retState;
            retState = Teche::ControlSdk::GetCurrentCameraState(&currentCameraInfo);
            if (retState)   //获取状态成功
            {
                qDebug() << "触发类型：" << mWorkType.value(currentCameraInfo.workType);
                qDebug() << "相片质量：" << mQualityType.value(currentCameraInfo.photoQuality);
                qDebug() << "分辨率："
                         << "宽："
                         << currentCameraInfo.width
                         << "高: "
                         << currentCameraInfo.height;

                //白平衡模式
                QString wbMode;

                if (currentCameraInfo.titanWbInfo.mode == Teche::Follow)
                {
                    wbMode       = "统一";
                    int wbMaster = currentCameraInfo.titanWbInfo.cameraList[0].cameraIndex;
                    qDebug() << "白平衡： 模式： 统一  主镜头：" << wbMaster << "    类型：自动";
                }
                else
                {
                    qDebug() << "白平衡： 模式：独立";
                    for (int i = 0; i < currentCameraInfo.titanWbInfo.cameraList.size(); i++)
                    {
                        if (currentCameraInfo.titanWbInfo.cameraList[i].type == Teche::WbAuto)
                        {
                            qDebug() << "镜头索引：" << i << "    类型: 自动";
                        }
                        else
                        {
                            qDebug() << "镜头索引："
                                     << i
                                     << " 类型：手动"
                                     << " r:"
                                     << currentCameraInfo.titanWbInfo.cameraList[i].r
                                     << " b:"
                                     << currentCameraInfo.titanWbInfo.cameraList[i].b
                                     << " gr:"
                                     << currentCameraInfo.titanWbInfo.cameraList[i].gr
                                     << " gb:"
                                     << currentCameraInfo.titanWbInfo.cameraList[i].gb;
                        }
                    }
                }

                //曝光模式
                std::string exMode;
                if (currentCameraInfo.titanExposureInfo.mode == Teche::parameterMode::Follow)
                {
                    exMode               = "统一";
                    int         wbMaster = currentCameraInfo.titanExposureInfo.cameraList[0].cameraIndex;
                    std::string exType   = "";
                    if (currentCameraInfo.titanExposureInfo.cameraList[0].type == Teche::parameterType::Auto)
                    {
                        exType = "自动";
                        std::cout << "曝光： 模式：" << exMode << "   主镜头：" << wbMaster << "    类型：" << exType << "   Compensation:" << currentCameraInfo.titanExposureInfo.cameraList[0].compensation << std::endl;
                    }
                    else if (currentCameraInfo.titanExposureInfo.cameraList[0].type == Teche::parameterType::ISOPriority)
                    {
                        exType = "ISO优先";
                        std::cout << "曝光： 模式：" << exMode << "   主镜头：" << wbMaster << "    类型：" << exType << "   Compensation:" << currentCameraInfo.titanExposureInfo.cameraList[0].compensation << "   ISO:" << currentCameraInfo.titanExposureInfo.cameraList[0].iso << std::endl;
                    }
                    else if (currentCameraInfo.titanExposureInfo.cameraList[0].type == Teche::parameterType::ShutterPriority)
                    {
                        exType = "快门优先";
                        std::cout << "曝光： 模式：" << exMode << "   主镜头：" << wbMaster << "    类型：" << exType << "   Compensation:" << currentCameraInfo.titanExposureInfo.cameraList[0].compensation << "   曝光时间:" << currentCameraInfo.titanExposureInfo.cameraList[0].expo << std::endl;
                    }
                    else
                    {
                        exType = "手动";
                        std::cout << "曝光： 模式：" << exMode << "   主镜头：" << wbMaster << "    类型：" << exType << "   ISO:" << currentCameraInfo.titanExposureInfo.cameraList[0].iso << "   曝光时间:" << currentCameraInfo.titanExposureInfo.cameraList[0].expo << std::endl;
                    }
                }
                else
                {
                    exMode = "独立";
                    std::cout << "曝光： 模式：" << exMode << std::endl;
                    std::string wbType;
                    for (int i = 0; i < currentCameraInfo.titanExposureInfo.cameraList.size(); i++)
                    {

                        std::string exType = "";
                        if (currentCameraInfo.titanExposureInfo.cameraList[i].type == Teche::parameterType::Auto)
                        {
                            exType = "自动";
                            std::cout << "镜头索引：" << i << "    类型：" << exType << "   Compensation:" << currentCameraInfo.titanExposureInfo.cameraList[i].compensation << std::endl;
                        }
                        else if (currentCameraInfo.titanExposureInfo.cameraList[i].type == Teche::parameterType::ISOPriority)
                        {
                            exType = "ISO优先";
                            std::cout << "镜头索引：" << i << "    类型：" << exType << "   Compensation:" << currentCameraInfo.titanExposureInfo.cameraList[i].compensation << "   ISO:" << currentCameraInfo.titanExposureInfo.cameraList[i].iso << std::endl;
                        }
                        else if (currentCameraInfo.titanExposureInfo.cameraList[i].type == Teche::parameterType::ShutterPriority)
                        {
                            exType = "快门优先";
                            std::cout << "镜头索引：" << i << "    类型：" << exType << "   Compensation:" << currentCameraInfo.titanExposureInfo.cameraList[i].compensation << "   曝光时间:" << currentCameraInfo.titanExposureInfo.cameraList[i].expo << std::endl;
                        }
                        else
                        {
                            exType = "手动";
                            std::cout << "镜头索引：" << i << "    类型：" << exType << "   ISO:" << currentCameraInfo.titanExposureInfo.cameraList[i].iso << "   曝光时间:" << currentCameraInfo.titanExposureInfo.cameraList[i].expo << std::endl;
                        }
                    }
                }

                if (currentCameraInfo.outputPulseEnable)   //开启脉冲输出
                {
                    std::string pulsePolarity = "";
                    if (currentCameraInfo.pulsePolarity == 0)
                    {
                        pulsePolarity = "低";
                    }
                    else
                    {
                        pulsePolarity = "高";
                    }

                    std::cout << "脉冲输出："
                              << "已开启" << std::endl;
                    std::cout << "脉冲宽度：" << currentCameraInfo.pulseWidth << "毫秒     脉冲延迟：" << currentCameraInfo.pulseDelay << "毫秒     脉冲极性：" << pulsePolarity << std::endl;
                }
                else   //外部曝光未开启
                {
                    std::cout << "脉冲输出："
                              << "未开启" << std::endl;
                }

                if (currentCameraInfo.isWork)
                {
                    std::cout << "相机状态："
                              << "工作中" << std::endl;

                inputWorkingOrder:
                    std::cout << "相机工作中！请输入下列命令进行操作：" << std::endl;
                    std::cout << "1.按当前工作模式保存数据；" << std::endl;
                    std::cout << "2.停止工作，重新设置相机工作模式；" << std::endl;
                    std::cin >> inputStr;

                    if (inputStr == "1")
                    {
                        std::cout << "请输入保存数据的路径：" << std::endl;
                    isWorkInputSavePath:
                        std::cin >> inputStr;
                        if (_access(inputStr.c_str(), 0) == -1)   //检测输入文件夹
                        {
                            std::cout << "输入的保存数据路径不存在！请重新输入：" << std::endl;
                            goto isWorkInputSavePath;
                        }
                        goto inputWorkingSavePath;
                    }
                    else if (inputStr == "2")
                    {
                        retState = Teche::ControlSdk::SetOperation(Teche::CameraOperation::Stop);
                        if (retState)
                        {
                            std::cout << "停止相机工作成功！" << std::endl;
                        }
                        else
                        {
                            std::cout << "停止相机工作失败！请查询相机工作状态！" << std::endl;
                        }
                        goto inputOrder;
                    }
                    else
                    {
                        goto inputWorkingOrder;
                    }

                    //关闭连接  或直接进入保存数据状态
                }
                else
                {
                    std::cout << "相机状态："
                              << "空闲" << std::endl;
                }

                goto inputOrder;
            }
            else
            {
                std::cout << "获取当前工作状态失败！请联系技术人员。" << std::endl;
                goto inputOrder;
            }
        }
        else
        {
            std::cout << "相机已经断开连接！请重新进行连接!" << std::endl;
            goto QueryCamera;
        }
    }
    else
    {
        qWarning() << "相机已经断开连接!请重新进行连接。";
    }
}

void Demo::slotGPSInfo()
{
}

void Demo::slotGrabImage(bool isGrab)
{
}

void Demo::slotSetOutput()
{
}

void Demo::slotSetSerialPort()
{
}

void Demo::slotSetWhiteBalance()
{
}

void Demo::slotSetExposure()
{
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
}
