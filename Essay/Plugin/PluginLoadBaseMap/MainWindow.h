#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class CustomListWidget;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadPlugins();
public slots:
    //    void slotLoadPlugin();
    //    void slotLoadBaseMap();
    //    void slotSupportBaseMap();
    void slotAddImage(const QString &name, const QPixmap &pixmap, const QPointF &pos);
    void slotChangeImageLevel(const QString &name, qreal level);

private:
    Ui::MainWindow *ui;

    QMap<QString, QGraphicsPixmapItem *> mPixmaps;

    QDockWidget *     mDockWidget;
    CustomListWidget *mListWidget;
    QGraphicsView *   mCenterView;
    QGraphicsScene *  mScene;
};

#endif   // MAINWINDOW_H
