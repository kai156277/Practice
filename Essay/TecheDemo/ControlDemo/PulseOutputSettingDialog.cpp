#include "PulseOutputSettingDialog.h"

#include "ui_PulseOutputSettingDialog.h"
#pragma execution_character_set("utf-8")

PulseOutputSettingDialog::PulseOutputSettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PulseOutputSettingDialog)
{
    ui->setupUi(this);
}

PulseOutputSettingDialog::~PulseOutputSettingDialog()
{
    delete ui;
}

void PulseOutputSettingDialog::on_buttonBox_accepted()
{
    pulseInfo.enable = ui->mPluseEnableCheckBox->isChecked();
    pulseInfo.delay  = ui->mPulseDelaySpinBox->value();
    pulseInfo.width  = ui->mPulseWidthSpinBox->value();

    if (ui->mPolarityHightRadioButton->isChecked())
        pulseInfo.polarity = 1;
    else
        pulseInfo.polarity = 0;
}
