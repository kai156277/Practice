#ifndef TESTPLUGINGLOBAL_H
#define TESTPLUGINGLOBAL_H

#ifdef PLUGIN_EXPORT_LIBRARY
#    define PLUGINCORE_API __declspec(dllexport)
#else
#    define PLUGINCORE_API __declspec(dllimport)
#endif

#endif   // TESTPLUGINGLOBAL_H
