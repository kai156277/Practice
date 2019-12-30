#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H

#include "BaseMapInterface.h"

#include <QListWidget>

class CustomListWidget : public QListWidget
{
    Q_OBJECT
public:
    CustomListWidget(QWidget *parent = nullptr);

signals:
    void addImage(const QString &name, const QPixmap &image, const QPointF &pos);
    void changeImageLevel(const QString &name, qreal level);

protected:
#if 0
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
#endif
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    //    void dragMoveEvent(QDragMoveEvent *event) override;

protected slots:
    void slotUpdateImageLevel();

private:
    void loadPlugins();

    QMap<QString, BaseMapInterface *> mLoaders;
    QStringList                       mFilters;
    QPoint                            mDragStartPos;
};

#endif   // CUSTOMLISTWIDGET_H
