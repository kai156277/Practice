TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Test.cpp \
        main.cpp

INCLUDEPATH += C:/ThirdPartyLib/libfmt-5.3.0/include
DEPENDPATH += C:/ThirdPartyLib/libfmt-5.3.0/include
DEFINES += FMT_HEADER_ONLY

INCLUDEPATH += C:/ThirdPartyLib/libspdlog-1.3.1/include
DEPENDPATH += C:/ThirdPartyLib/libspdlog-1.3.1/include

#DEFINES += SPDLOG_FMT_EXTERNAL

HEADERS += \
    Test.h \
    TestSpdlog.h
