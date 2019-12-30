QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = AnalogClock
CONFIG += console
CONFIG += app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    AnalogClock.cpp

HEADERS += \
    AnalogClock.h

doc.files += $$PWD/README.md
doc.path += $$OUT_PWD

INSTALLS += doc
