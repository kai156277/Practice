#include "BrozeStyleWidget.h"
#include "ui_BrozeStyleWidget.h"

BrozeStyleWidget::BrozeStyleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrozeStyleWidget)
{
    ui->setupUi(this);
}

BrozeStyleWidget::~BrozeStyleWidget()
{
    delete ui;
}
