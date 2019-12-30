TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    DynamicLib.cpp \
    PluginManager.cpp

DEFINES += PLUGIN_EXPORT_LIBRARY

HEADERS += \
    DynamicLib.h \
    Export.h \
    PluginInstance.h \
    PluginManager.h
