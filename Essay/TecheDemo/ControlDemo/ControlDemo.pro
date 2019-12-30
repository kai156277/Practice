#-------------------------------------------------
#
# Project created by QtCreator 2019-04-15T09:32:46
#
#-------------------------------------------------

QT       += core gui
include(../basePro.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ControlDemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += console

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        Demo.cpp \
    WhiteBalanceSettingDialog.cpp \
    ExposureSettingDialog.cpp \
    PulseOutputSettingDialog.cpp

HEADERS += \
        Demo.h \
    WhiteBalanceSettingDialog.h \
    ExposureSettingDialog.h \
    PulseOutputSettingDialog.h

FORMS += \
        Demo.ui \
    WhiteBalanceSettingDialog.ui \
    ExposureSettingDialog.ui \
    PulseOutputSettingDialog.ui

win32:CONFIG(release, debug|release) LIBS += -LC:/ThirdPartyLib/libTecha/lib/ -lControlSdk
else:win32:CONFIG(debug, debug|release) LIBS += -LC:/ThirdPartyLib/libTecha/lib/ -lControlSdkd

LIBS += -LC:/ThirdPartyLib/libTecha/opencvLib/ -lopencv_core320 -lopencv_highgui320 -lopencv_imgproc320

INCLUDEPATH += C:/ThirdPartyLib/libTecha/include
DEPENDPATH += C:/ThirdPartyLib/libTecha/include
