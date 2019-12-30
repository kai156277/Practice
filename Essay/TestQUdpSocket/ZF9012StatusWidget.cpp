#include "ZF9012StatusWidget.h"

#include "ui_ZF9012StatusWidget.h"

ZF9012StatusWidget::ZF9012StatusWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZF9012StatusWidget)
{
    ui->setupUi(this);
    mItems.insert(ZF9012Status_ScanningTime,
                  ItemPair {ui->scanningTimeLabel, ui->scanningTimeValue});
    mItems.insert(ZF9012Status_NumOfLines,
                  ItemPair {ui->numOfLinesLabel, ui->numOfLinesValue});
    mItems.insert(ZF9012Status_Progress,
                  ItemPair {ui->progressLabel, ui->progressValue});
    mItems.insert(ZF9012Status_FileSize,
                  ItemPair {ui->fileSizeLabel, ui->fileSizeValue});
    mItems.insert(ZF9012Status_PixelPerSec,
                  ItemPair {ui->pixelPerSecondLabel, ui->pixelPerSecondValue});
    mItems.insert(ZF9012Status_LinesPerSec,
                  ItemPair {ui->linesPerSecondLabel, ui->linesPerSecondValue});
    mItems.insert(ZF9012Status_MaxRange,
                  ItemPair {ui->maxRangeLabel, ui->maxRangeValue});
    mItems.insert(ZF9012Status_Filter,
                  ItemPair {ui->filterLabel, ui->filterValue});
    mItems.insert(ZF9012Status_HorzPosition,
                  ItemPair {ui->currentHorzPositionLabel, ui->currentHorzPositionValue});
    mItems.insert(ZF9012Status_NumOfErrors,
                  ItemPair {ui->numberOfErrorsLabel, ui->numberOfErrorsValue});
    mItems.insert(ZF9012Status_Step,
                  ItemPair {ui->currentStepLabel, ui->currentStepValue});
    mItems.insert(ZF9012Status_ReferenzRange,
                  ItemPair {ui->referenzRangeLabel, ui->referenzRangeValue});
    mItems.insert(ZF9012Status_XLevel,
                  ItemPair {ui->xLevelLabel, ui->xLevelValue});
    mItems.insert(ZF9012Status_YLevel,
                  ItemPair {ui->yLevelLabel, ui->yLevelValue});
}

ZF9012StatusWidget::~ZF9012StatusWidget()
{
    delete ui;
}

void ZF9012StatusWidget::setItems(const QVector<PackageItem> &items)
{
    setAllLabelVisable(false);
    for (int i = 0; i < items.size(); ++i)
    {
        PackageItem          item      = items[i];
        ZF9012StatusItemType itemIndex = (ZF9012StatusItemType) item.index;
        if (mItems.contains(itemIndex))
        {
            mItems[itemIndex].key->setVisible(true);
            mItems[itemIndex].value->setVisible(true);
            switch (item.type)
            {
            case 1:
                mItems[itemIndex].value->setText(QString::number(item.value._int32Value));
                break;
            case 2:
                mItems[itemIndex].value->setText(QString::number(item.value._doubleValue));
                break;
            case 3:   // use setSetp
                break;
            case 4:
                mItems[itemIndex].value->setText(QString::number(item.value._groupValue.second));
            default:
                break;
            }
        }
    }
}

void ZF9012StatusWidget::setStep(const QString &step)
{
    ui->currentHorzPositionLabel->setVisible(true);
    ui->currentHorzPositionValue->setVisible(true);
    ui->currentHorzPositionValue->setText(step);
}

void ZF9012StatusWidget::setAllLabelVisable(bool flag)
{
    QObjectList list = ui->gridLayout->children();
    for (QObject *obj : list)
    {
        QLabel *label = qobject_cast<QLabel *>(obj);
        if (label)
            label->setVisible(flag);
    }
}
