#include "BronzeStyle.h"

BronzeStyle::BronzeStyle()
{
}

void BronzeStyle::polish(QPalette &palette)
{
    QPixmap backgroundImage(":/images/backgraound.png");
    QColor  bronze(207, 155, 95);
    QColor  veryLightBlue(239, 239, 247);
    QColor  lightBlue(223, 223, 239);
    QColor  darkBlue(95, 95, 191);

    palette = QPalette(bronze);
    palette.setBrush(QPalette::Window, backgroundImage);
    palette.setBrush(QPalette::BrightText, Qt::white);
    palette.setBrush(QPalette::Base, veryLightBlue);
    palette.setBrush(QPalette::AlternateBase, lightBlue);
    palette.setBrush(QPalette::Highlight, darkBlue);
    palette.setBrush(QPalette::Disabled, QPalette::Highlight, Qt::darkGray);
}
