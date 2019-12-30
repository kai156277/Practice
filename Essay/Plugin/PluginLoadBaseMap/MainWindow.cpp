#include "MainWindow.h"

#include "BaseMapInterface.h"
#include "CustomListWidget.h"
#include "ui_MainWindow.h"

#include <QPluginLoader>
#include <QtCore>
#include <QtPlugin>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mDockWidget = new QDockWidget("CoustomListWidget");
    addDockWidget(Qt::LeftDockWidgetArea, mDockWidget);

    mListWidget = new CustomListWidget(this);
    mDockWidget->setWidget(mListWidget);

    mScene      = new QGraphicsScene;
    mCenterView = new QGraphicsView(mScene);
    mScene->setSceneRect(0, 0, 500, 500);
    mScene->setBackgroundBrush(QBrush(Qt::gray));
    mCenterView->setDragMode(QGraphicsView::ScrollHandDrag);
    mCenterView->setMouseTracking(true);
    setCentralWidget(mCenterView);

    connect(mListWidget, &CustomListWidget::addImage, this, &MainWindow::slotAddImage);
    connect(mListWidget, &CustomListWidget::changeImageLevel, this, &MainWindow::slotChangeImageLevel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPlugins()
{
}

void MainWindow::slotAddImage(const QString &name, const QPixmap &pixmap, const QPointF &pos)
{
    QGraphicsPixmapItem *item = mScene->addPixmap(pixmap);
    item->setPos(pos);
    mPixmaps.insert(name, item);
}

void MainWindow::slotChangeImageLevel(const QString &name, qreal level)
{
    QGraphicsPixmapItem *item = mPixmaps.value(name, nullptr);
    if (item)
    {
        item->setZValue(level);
    }
}
