#ifndef CANVAS_H
#define CANVAS_H

#include <QPolygon>
#include <QWidget>

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = 0);
    ~Canvas();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private slots:
    void slotAddPoint();
    void slotGetConyexHull();
    void slotClearPoint();

private:
    Ui::Canvas *ui;

    QPolygon mPoints;
    QPoint   mMousePos;
    QPolygon mConyexHullPoints;
};

#endif   // CANVAS_H
