#ifndef PLUGININSTANCE_H
#define PLUGININSTANCE_H

#include "Export.h"

#include <string>

class PLUGINCORE_API PluginInstance
{
public:
    explicit PluginInstance(const std::string &strName) {}
    virtual ~PluginInstance() {}

    virtual bool load()   = 0;
    virtual bool unLoad() = 0;

    virtual std::string getFileName() const    = 0;
    virtual std::string getDisplayName() const = 0;

private:
    PluginInstance(const PluginInstance &rhs);
    const PluginInstance &operator=(const PluginInstance &rhs);
};

typedef PluginInstance *(*START_PLUGIN_FUN)();
typedef void (*STOP_PLUGIN_FUN)();

extern "C" PLUGINCORE_API PluginInstance *StartPlugin();
extern "C" PLUGINCORE_API void            StopPlugin();

#endif   // PLUGININSTANCE_H
