#ifndef WHITEBALANCESETTINGDIALOG_H
#define WHITEBALANCESETTINGDIALOG_H

#include <ControlSdk.h>
#include <QDialog>
#include <vector>

class QButtonGroup;
namespace Ui {
class WhiteBalanceSettingDialog;
}

class WhiteBalanceSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WhiteBalanceSettingDialog(QWidget *parent = 0);
    ~WhiteBalanceSettingDialog();
    Teche::TitansWbInfo WbInfo;
public slots:
    void slotWhiteBalanceApple();
    void slotWbTypeIndexChanged(int index);

private slots:
    void on_buttonBox_accepted();

private:
    std::vector<Teche::CameraWbInfo> mCameraList;   //镜头白平衡参数队列 如果是跟随模式就全是自动，队列一个镜头是主镜头，如果是独立就设置哪个哪个手动，没设置的自动
    Ui::WhiteBalanceSettingDialog *  ui;
    QButtonGroup *                   mGroup = nullptr;
};

#endif   // WHITEBALANCESETTINGDIALOG_H
