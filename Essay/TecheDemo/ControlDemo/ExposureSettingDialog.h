#ifndef EXPOSURESETTINGDIALOG_H
#define EXPOSURESETTINGDIALOG_H

#include <ControlSdk.h>
#include <QDialog>

namespace Ui {
class ExposureSettingDialog;
}

class ExposureSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExposureSettingDialog(QWidget *parent = 0);
    ~ExposureSettingDialog();

    Teche::TitansExposureInfo exposure;
private slots:
    void on_mApplyPushButton_clicked();

    void on_buttonBox_accepted();

private:
    std::vector<Teche::CameraExposureInfo> mCameraList;
    Ui::ExposureSettingDialog *            ui;
};

#endif   // EXPOSURESETTINGDIALOG_H
