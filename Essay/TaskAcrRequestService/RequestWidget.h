#ifndef REQUESTWIDGET_H
#define REQUESTWIDGET_H

#include "RequestModule.h"

#include <QWidget>

namespace Ui {
class RequestWidget;
}

class RequestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequestWidget(QWidget *parent = 0);
    ~RequestWidget();

private slots:
    void on_mRequestAttributes_clicked();

    void on_mRequestData_clicked();

private:
    Ui::RequestWidget *      ui;
    vsr::acr::RequestModule *module;
};

#endif   // REQUESTWIDGET_H
