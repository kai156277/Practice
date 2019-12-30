#ifndef BROZESTYLEWIDGET_H
#define BROZESTYLEWIDGET_H

#include <QWidget>

namespace Ui {
class BrozeStyleWidget;
}

class BrozeStyleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrozeStyleWidget(QWidget *parent = 0);
    ~BrozeStyleWidget();

private:
    Ui::BrozeStyleWidget *ui;
};

#endif // BROZESTYLEWIDGET_H
