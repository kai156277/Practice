#-------------------------------------------------
#
# Project created by QtCreator 2019-03-18T11:27:36
#
#-------------------------------------------------

QT       += core gui
include(../baselib.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TaskAcrRequestService
TEMPLATE = app

CONFIG += console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:CONFIG(release, debug|release): LIBS += -L'C:/ThirdPartyLib/libAcr0/lib/' -lacr -lrpc
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/ThirdPartyLib/libAcr0/lib/' -lacrd -lrpcd

INCLUDEPATH += "C:/ThirdPartyLib/libAcr0/include"
DEPENDPATH += "C:/ThirdPartyLib/libAcr0/include"

win32:CONFIG(release, debug|release): LIBS += -L"C:/ThirdPartyLib/libjsoncpp/lib" -ljsoncpp
else:win32:CONFIG(debug, debug|release): LIBS += -L"C:/ThirdPartyLib/libjsoncpp/lib" -ljsoncppd

INCLUDEPATH += 'C:/ThirdPartyLib/libjsoncpp/include'
DEPENDPATH += 'C:/ThirdPartyLib/libjsoncpp/include'

SOURCES += \
        main.cpp \
        RequestWidget.cpp \
    RequestModule.cpp

HEADERS += \
        RequestWidget.h \
    RequestModule.h

FORMS += \
        RequestWidget.ui
