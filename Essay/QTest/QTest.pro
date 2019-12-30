QT += core gui widgets

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DataManager.cpp \
        DataSource.cpp \
        DataSourceFactory.cpp \
        Dialog.cpp \
        PrintFormat.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    DataManager.h \
    DataSource.h \
    DataSourceFactory.h \
    Dialog.h \
    PrintFormat.h

INCLUDEPATH += C:/ThirdPartyLib/libfmt-5.3.0/include
DEPENDPATH += C:/ThirdPartyLib/libfmt-5.3.0/include
DEFINES += FMT_HEADER_ONLY

INCLUDEPATH += C:/ThirdPartyLib/libspdlog-1.3.1/include
DEPENDPATH += C:/ThirdPartyLib/libspdlog-1.3.1/include
DEFINES += SPDLOG_FMT_EXTERNAL

RC_FILE += resource.rc

FORMS += \
    Dialog.ui
