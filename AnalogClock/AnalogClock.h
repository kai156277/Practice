#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include <QWidget>

class QPaintEvent;
class AnalogClock : public QWidget
{
    Q_OBJECT
public:
    explicit AnalogClock(QWidget *parent = 0);

protected slots:
    void paintEvent(QPaintEvent* event);
};

#endif // ANALOGCLOCK_H
