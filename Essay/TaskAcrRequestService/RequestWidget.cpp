#include "RequestWidget.h"

#include "ui_RequestWidget.h"

#include <acr/module/ModuleAgent.h>

using namespace vsr;
using namespace vsr::acr;

RequestWidget::RequestWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RequestWidget)
{
    ui->setupUi(this);
    ModuleMeta meta;

    meta.name   = "RequestModule";
    meta.vender = "VSurs";
    meta.series = "Test-RequestService";
    meta.serial = "XiuShanMobieSurvey";

    module = new RequestModule(meta, "192.168.1.3", 2222, 1);
    module->initializeModuleDiscovery();
    module->asynchRunService();
}

RequestWidget::~RequestWidget()
{
    delete ui;
}

void RequestWidget::on_mRequestAttributes_clicked()
{
    module->sendRequestAttributes();
}

void RequestWidget::on_mRequestData_clicked()
{
    module->sendRequestValues();
}
