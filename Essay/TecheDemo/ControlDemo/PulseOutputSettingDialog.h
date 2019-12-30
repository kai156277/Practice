#ifndef PULSEOUTPUTSETTINGDIALOG_H
#define PULSEOUTPUTSETTINGDIALOG_H

#include <ControlSdk.h>
#include <QDialog>

namespace Ui {
class PulseOutputSettingDialog;
}

class PulseOutputSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PulseOutputSettingDialog(QWidget *parent = 0);
    ~PulseOutputSettingDialog();

    Teche::PulseOutputInfo pulseInfo;
private slots:
    void on_buttonBox_accepted();

private:
    Ui::PulseOutputSettingDialog *ui;
};

#endif   // PULSEOUTPUTSETTINGDIALOG_H
