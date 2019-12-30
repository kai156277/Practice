#ifndef ZF9012STATUSWIDGET_H
#define ZF9012STATUSWIDGET_H

#include "ZF9012StatusDef.h"

#include <QMap>
#include <QWidget>

namespace Ui {
class ZF9012StatusWidget;
}

class QLabel;
class ZF9012StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZF9012StatusWidget(QWidget *parent = 0);
    ~ZF9012StatusWidget();

    void setItems(const QVector<PackageItem> &items);
    void setStep(const QString &step);

private:
    void setAllLabelVisable(bool flag);

    Ui::ZF9012StatusWidget *ui;

    struct ItemPair
    {
        QLabel *key;
        QLabel *value;
    };
    QMap<ZF9012StatusItemType, ItemPair> mItems;
};

#endif   // ZF9012STATUSWIDGET_H
