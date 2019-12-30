#include "WhiteBalanceSettingDialog.h"

#include "ui_WhiteBalanceSettingDialog.h"

#include <ControlSdk.h>
#include <QtCore>
#include <QtGui>

#pragma execution_character_set("utf-8")

WhiteBalanceSettingDialog::WhiteBalanceSettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WhiteBalanceSettingDialog)
{
    ui->setupUi(this);
    mGroup = new QButtonGroup(this);
    mGroup->addButton(ui->mFollowRadioButton);
    mGroup->addButton(ui->mIndependentRadioButton);

    QStringList cameras = {"0号相机", "1号相机", "2号相机", "3号相机", "4号相机", "5号相机"};
    ui->mMainIndexComboBox->addItems(cameras);
    ui->mCameraIndexComboBox->addItems(cameras);

    ui->mWBTypeComboBox->addItem("自动", Teche::WbAuto);
    ui->mWBTypeComboBox->addItem("手动", Teche::WbManual);

    connect(ui->mIndependentApplyPushButton,
            &QPushButton::clicked,
            this,
            &WhiteBalanceSettingDialog::slotWhiteBalanceApple);

    connect(ui->mWBTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(slotWbTypeIndexChanged(int)));

    mCameraList.reserve(6);
    for (int i = 0; i < 6; i++)
    {
        Teche::CameraWbInfo camera;
        camera.cameraIndex = i;
        camera.type        = Teche::WbManual;
        camera.b           = 512;
        camera.r           = 512;
        camera.gr          = 256;
        camera.gb          = 256;

        mCameraList.push_back(camera);
    }
}

WhiteBalanceSettingDialog::~WhiteBalanceSettingDialog()
{
    delete ui;
}

void WhiteBalanceSettingDialog::slotWhiteBalanceApple()
{
    int index = ui->mCameraIndexComboBox->currentIndex();

    if (0 <= index && index < mCameraList.size())
    {
        mCameraList[index].b  = ui->mWBbSpinBox->value();
        mCameraList[index].r  = ui->mWBrSpinBox->value();
        mCameraList[index].gb = ui->mWBgbSpinBox->value();
        mCameraList[index].gr = ui->mWBgrSpinBox->value();
    }
}

void WhiteBalanceSettingDialog::slotWbTypeIndexChanged(int index)
{
    bool ok = false;

    Teche::wbParameterType type = (Teche::wbParameterType) ui->mWBTypeComboBox->currentData().toInt(&ok);

    qDebug() << (ok ? "成功转为参数类型" : "未成功转为参数类型");

    for (int i = 0; i < 6; i++)
    {
        mCameraList[i].type = type;
    }
}

void WhiteBalanceSettingDialog::on_buttonBox_accepted()
{
    WbInfo.cameraList.clear();
    if (ui->mFollowRadioButton->isChecked())
    {
        WbInfo.mode = Teche::Follow;
        Teche::CameraWbInfo camera;
        camera.cameraIndex = ui->mMainIndexComboBox->currentIndex();
        camera.type        = Teche::WbAuto;
        camera.b           = 512;
        camera.r           = 512;
        camera.gr          = 256;
        camera.gb          = 256;
        WbInfo.cameraList.push_back(camera);
        return;
    }

    if (ui->mIndependentRadioButton->isChecked())
    {
        WbInfo.mode       = Teche::Independent;
        WbInfo.cameraList = mCameraList;
        return;
    }
}
