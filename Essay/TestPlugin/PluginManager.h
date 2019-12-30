#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "Export.h"

#include <map>
#include <string>
#include <vector>

class PluginInstance;
class DynamicLib;

class PLUGINCORE_API PluginManager
{
public:
    static PluginManager &getInstance();

    bool            loadAll();
    PluginInstance *load(const std::string &name, int &errCode);
    bool            loadPlugin(PluginInstance *plugin);
    bool            unLoadAll();
    bool            unLoad(const std::string &name);
    bool            unLoadPlugin(PluginInstance *plugin);

    std::vector<PluginInstance *> getAllPlugins();

private:
    PluginManager();
    ~PluginManager();
    PluginManager(const PluginManager &rhs);
    const PluginManager &operator=(const PluginManager &rhs);

    std::vector<PluginInstance *>       mPlugins;
    std::map<std::string, DynamicLib *> mPluginLibs;
};

#endif   // PLUGINMANAGER_H
