#pragma once   // BRONZESTYLE_H

#include <QWindowsStyle>

class BronzeStyle : public QWindowsStyle
{
    Q_OBJECT
public:
    BronzeStyle();

    void polish(QPalette &palette) override;
    void polish(QWidget *widget) override;
    void unpolish(QWidget *widget) override;

    int styleHint(StyleHint           hint,
                  const QStyleOption *opt,
                  const QWidget *     widget     = 0,
                  QStyleHintReturn *  returnData = 0) const override;
    int pixelMetric(PixelMetric         pm,
                    const QStyleOption *option,
                    const QWidget *     widget = 0) const override;

    void drawPrimitive(PrimitiveElement    pe,
                       const QStyleOption *opt,
                       QPainter *          p,
                       const QWidget *     w = 0) const override;
    void drawComplexControl(ComplexControl             cc,
                            const QStyleOptionComplex *opt,
                            QPainter *                 p,
                            const QWidget *            w = 0) const override;

    QRect subControlRect(ComplexControl             cc,
                         const QStyleOptionComplex *opt,
                         SubControl                 sc,
                         const QWidget *            w = 0) const override;
public slots:
    QIcon standardIconImplementation(StandardPixmap      standardIcon,
                                     const QStyleOption *option,
                                     const QWidget *     widget = 0) const;

private:
    void drawBronzeFrame(const QStyleOption *option, QPainter *painter) const;
    void drawBronzeBevel(const QStyleOption *option, QPainter *painter) const;
    void drawBronzeCheckBoxIndicator(const QStyleOption *option, QPainter *painter) const;
    void drawBronzeSpinBoxButton(SubControl                 which,
                                 const QStyleOptionComplex *option,
                                 QPainter *                 painter) const;
};
