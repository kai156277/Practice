#include "Demo.h"

#include "ui_Demo.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <SwitchDll.h>
#include <iostream>

#pragma execution_character_set("utf-8")

Q_DECLARE_METATYPE(Teche::ConvertState)

using namespace Teche;

Demo *smDemo = nullptr;
Demo::Demo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Demo)
{
    ui->setupUi(this);
    smDemo = this;
    QSettings config;

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(ui->mConvertFileCheckBox);
    group->addButton(ui->mConvertPanoramaCheckBox);

    ui->mStartPushButton->setEnabled(true);
    ui->mStopPushButton->setEnabled(false);

    ui->mInputFolderLineEdit->setReadOnly(true);
    ui->mSerialPortFileLineEdit->setReadOnly(true);
    ui->mCalibrationFileLineEdit->setReadOnly(true);

    ui->mConvertPanoramaCheckBox->setChecked(true);
    ui->mSerialPortFileCheckBox->setChecked(true);
    ui->mNoiseCheckBox->setChecked(true);

    if (!config.contains("calibrationFile") || config.value("calibrationFile").isNull())
        slotSetCalibrationFile();
    else
        ui->mCalibrationFileLineEdit->setText(config.value("calibrationFile").toString());

    connect(ui->mInputFolderPushButton, &QPushButton::clicked, this, &Demo::slotSetInputFolder);
    connect(ui->mCalibrationFilePushButton, &QPushButton::clicked, this, &Demo::slotSetCalibrationFile);
    connect(ui->mSerialPortPushButton, &QPushButton::clicked, this, &Demo::slotSetSerialPortFile);
    connect(ui->mStartPushButton, &QPushButton::clicked, this, &Demo::slotStartProcessTeFile);
    connect(ui->mStopPushButton, &QPushButton::clicked, this, &Demo::slotStopProcessTeFile);
    connect(ui->mCreateTimeFilePushButton, &QPushButton::clicked, this, &Demo::slotCreateTimeFile);

    connect(this, &Demo::saveImage, this, &Demo::slotSaveImage);
    qRegisterMetaType<Teche::ConvertState>("Teche::ConvertState");
}

Demo::~Demo()
{
    delete ui;
}

void Demo::slotSetInputFolder()
{
    QSettings config;
    QString   defFile  = config.value("inputPath", "").toString();
    QString   savepath = QFileDialog::getExistingDirectory(this,
                                                         "文件输入路径",
                                                         defFile,
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (savepath.isEmpty())
        return;

    config.setValue("inputPath", savepath);
    ui->mInputFolderLineEdit->setText(savepath);
}

void Demo::slotSetCalibrationFile()
{
    QSettings config;
    QString   defFile     = config.value("calibrationFile", "").toString();
    QString   calibration = QFileDialog::getOpenFileName(this, "标定文件", defFile, "标定文件 (*.xml)");
    if (calibration.isEmpty())
        return;
    config.setValue("calibrationFile", calibration);

    ui->mCalibrationFileLineEdit->setText(calibration);
}

void Demo::slotSetSerialPortFile()
{
    QSettings config;
    QString   defFile     = config.value("inputPath", "").toString();
    QString   calibration = QFileDialog::getOpenFileName(this, "串口信息文件", defFile, "串口信息文件 (*.txt)");
    if (calibration.isEmpty())
        return;
    config.setValue("inputPath", calibration);

    ui->mSerialPortFileLineEdit->setText(calibration);
}

void Demo::slotStartProcessTeFile()
{
    QString input = ui->mInputFolderLineEdit->text();
    QDir    folderChecker;
    folderChecker.setPath(input);
    if (!folderChecker.exists())
    {
        QMessageBox::critical(this, "Error!", "Input folder: " + input + " not exists！");
        return;
    }
    std::string _input(input.toLocal8Bit().constData());

    std::string _output = _input;

    QString   calibration = ui->mCalibrationFileLineEdit->text();
    QFileInfo fileChecker;
    fileChecker.setFile(calibration);
    if (!fileChecker.exists())
    {
        QMessageBox::critical(this, "Error!", "Calibration file: " + calibration + " not exists! ");
        return;
    }
    std::string _calibration(calibration.toLocal8Bit().constData());

    QString serialPort    = "";
    bool    useSerialPort = ui->mSerialPortFileCheckBox->isChecked();
    if (useSerialPort)
    {
        fileChecker.setFile(ui->mSerialPortFileLineEdit->text());
        if (!fileChecker.exists())
        {
            QMessageBox::critical(this, "Error!", "Serial Port file: " + serialPort + " not exists! ");
            return;
        }
        serialPort = ui->mSerialPortFileLineEdit->text();
    }
    std::string _serialPort(serialPort.toLocal8Bit().constData());

    auto tmp = [](int count, Teche::ConvertState state) {
        emit smDemo->saveImage(count, state);
    };
    if (ui->mConvertFileCheckBox->isChecked())
    {
        std::cout << "input:" << _input << std::endl;
        std::cout << "output:" << _output << std::endl;
        mConvertFile = new Teche::ConvertFile(_input, Teche::Folder, _serialPort, tmp);
        mConvertFile->SaveImage(_output, 90);
    }

    if (ui->mConvertPanoramaCheckBox->isChecked())
    {
        int  resolution = ui->mResolutionSpinBox->value();
        int  threadNum  = ui->mWorkThreadSpinBox->value();
        int  brightness = ui->mBrightnessSpinBox->value();
        bool noise      = ui->mNoiseCheckBox->isChecked();
        std::cout << "input:" << _input << std::endl;
        std::cout << "output:" << _output << std::endl;
        std::cout << "serialPort:" << _serialPort << std::endl;
        std::cout << "calibration:" << _calibration << std::endl;
        std::cout << "resolution:" << resolution << std::endl;
        std::cout << "threadNum:" << threadNum << std::endl;
        std::cout << "brightness:" << brightness << std::endl;
        std::cout << "noise:" << (noise ? "True" : "False") << std::endl;
        mConvertSwitchFile = new Teche::ConvertSwitchFile(_input, _serialPort, Teche::Folder, tmp);
        mConvertSwitchFile->SaveImage(_output, _calibration, resolution, 90, threadNum, brightness, noise);
    }

    ui->mConvertFileCheckBox->setEnabled(false);
    ui->mConvertPanoramaCheckBox->setEnabled(false);
    ui->mStartPushButton->setEnabled(false);
    ui->mStopPushButton->setEnabled(true);
    mBegin = QDateTime::currentDateTime();
    qDebug() << "处理开始时间:" << mBegin.toString("yyyy/MM/dd hh:mm:ss zzz");
    isStop = false;
    mCount = 1;
}

void Demo::slotStopProcessTeFile()
{
    if (isStop)
        return;

    isStop = true;

    mEnd = QDateTime::currentDateTime();
    qDebug() << "处理结束时间:" << mEnd.toString("yyyy/MM/dd hh:mm:ss zzz");
    int mes = mBegin.msecsTo(mEnd);
    qDebug() << "平均处理时间：" << ((double) mes / (double) mCount);
    if (ui->mConvertFileCheckBox->isChecked())
    {
        mConvertFile->Stop();
        delete mConvertFile;
        mConvertFile = nullptr;
    }
    if (ui->mConvertPanoramaCheckBox->isChecked())
    {
        mConvertSwitchFile->Stop();
        delete mConvertSwitchFile;
        mConvertSwitchFile = nullptr;
    }
    ui->mStartPushButton->setEnabled(true);
    ui->mStopPushButton->setEnabled(false);
    ui->mConvertFileCheckBox->setEnabled(true);
    ui->mConvertPanoramaCheckBox->setEnabled(true);

    if (ui->mCreateTimeFileCheckBox->isChecked())
    {
        QMessageBox::information(this, "信息", "生成时间文件！");
        slotCreateTimeFile();
    }
    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(ui->mInputFolderLineEdit->text()).toString(), QUrl::TolerantMode));
}

void Demo::slotSaveImage(int count, ConvertState state)
{
    qDebug() << "相片序号：" << count << "处理时间:" << QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss zzz");
    switch (state)
    {
    case Teche::Error:
    {
        ui->mMessageLabel->setText("图片生成报错！");
        break;
    }
    case Teche::InitSuccess:
    {
        ui->mMessageLabel->setText("图片初始化成功！");
        break;
    }
    case Teche::Saveing:
    {
        ui->mMessageLabel->setText("生成图片数量：" + QString::number(count));
        break;
    }
    case Teche::SaveSuccess:
    {
        ui->mMessageLabel->setText("生成图片数量：" + QString::number(count));
        mCount = count;
        slotStopProcessTeFile();
        QMessageBox::information(this, "成功", "生成相片成功！");
        break;
    }
    default:
        break;
    }
}

void Demo::slotCreateTimeFile()
{
    QSettings config;
    QString   defFile  = config.value("inputPath", "").toString();
    QString   savepath = QFileDialog::getExistingDirectory(this,
                                                         "文件输入路径",
                                                         defFile,
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (savepath.isEmpty())
        return;

    createTimeFile(savepath);
}

void Demo::createTimeFile(const QString &path)
{
    QDir          dir(path);
    QFileInfoList jpegs = dir.entryInfoList(QStringList() << "*.jpg", QDir::Files, QDir::Name);

    QFile time(path + "/" + dir.dirName() + ".time");
    if (time.open(QIODevice::WriteOnly))
    {
        QTextStream w(&time);
        for (const QFileInfo &jpeg : jpegs)
        {
            QDateTime time = QDateTime::fromString(jpeg.baseName(), "yyyyMMddhhmmsszzz_01");
            if (time.isValid())
            {
                w << jpeg.baseName() << "," << time.toString("yyyy,MM,dd,hh,mm,ss,zzz000") << endl;
            }
            else
            {
                QMessageBox::critical(this, "错误", "无法正确处理时间");
                break;
            }
        }
        time.close();
        QMessageBox::information(this, "成功", "生成time文件成功");
    }
    else
    {
        QMessageBox::critical(this, "错误", "无法打开time文件");
    }
}
