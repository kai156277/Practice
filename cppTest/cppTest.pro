TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -LC:/ThirdPartyLib/msvc141/libproj-6.2.1/lib/ -lproj_6_2
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/ThirdPartyLib/msvc141/libproj-6.2.1/lib/ -lproj_6_2_d

INCLUDEPATH += C:/ThirdPartyLib/msvc141/libproj-6.2.1/include
DEPENDPATH += C:/ThirdPartyLib/msvc141/libproj-6.2.1/include
