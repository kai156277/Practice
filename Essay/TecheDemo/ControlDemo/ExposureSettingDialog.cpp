#include "ExposureSettingDialog.h"

#include "ui_ExposureSettingDialog.h"
#pragma execution_character_set("utf-8")

ExposureSettingDialog::ExposureSettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExposureSettingDialog)
{
    ui->setupUi(this);

    QStringList cameras = {"0号相机", "1号相机", "2号相机", "3号相机", "4号相机", "5号相机"};
    ui->mCameraIndexComboBox->addItems(cameras);

    ui->mTypeComboBox->addItem("自动", Teche::Auto);
    ui->mTypeComboBox->addItem("手动", Teche::Manual);
    ui->mTypeComboBox->addItem("快门优先", Teche::ShutterPriority);
    ui->mTypeComboBox->addItem("ISO优先", Teche::ISOPriority);

    mCameraList.resize(6);
    for (int i = 0; i < mCameraList.size(); ++i)
    {
        mCameraList[i].cameraIndex  = i;
        mCameraList[i].type         = Teche::Manual;
        mCameraList[i].expo         = 1000;   // 曝光
        mCameraList[i].iso          = 100;
        mCameraList[i].compensation = 56;
    }
}

ExposureSettingDialog::~ExposureSettingDialog()
{
    delete ui;
}

void ExposureSettingDialog::on_mApplyPushButton_clicked()
{
    int index = ui->mCameraIndexComboBox->currentIndex();
    if (0 <= index && index < mCameraList.size())
    {
        mCameraList[index].cameraIndex  = index;
        mCameraList[index].type         = (Teche::parameterType) ui->mTypeComboBox->currentData().toInt();
        mCameraList[index].expo         = ui->mExpoSpinBox->value();
        mCameraList[index].iso          = ui->mISOSpinBox->value();
        mCameraList[index].compensation = ui->mCompensationSpinBox->value();
    }
}

void ExposureSettingDialog::on_buttonBox_accepted()
{
    if (ui->mIndependentRadioButton->isChecked())
    {
        exposure.mode       = Teche::Independent;
        exposure.cameraList = mCameraList;
    }
    else
    {
        exposure.mode = Teche::Follow;
        Teche::CameraExposureInfo camera;
        camera.type         = (Teche::parameterType) ui->mTypeComboBox->currentData().toInt();
        camera.expo         = ui->mExpoSpinBox->value();
        camera.iso          = ui->mISOSpinBox->value();
        camera.compensation = ui->mCompensationSpinBox->value();
        camera.cameraIndex  = ui->mCameraIndexComboBox->currentIndex();
        exposure.cameraList.push_back(camera);
    }
}
