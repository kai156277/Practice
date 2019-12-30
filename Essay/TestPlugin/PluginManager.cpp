#include "PluginManager.h"

#include "DynamicLib.h"
#include "PluginInstance.h"

using namespace std;

PluginManager &PluginManager::getInstance()
{
    static PluginManager instance;
    return instance;
}

bool PluginManager::loadAll()
{
    for (PluginInstance *plugin : mPlugins)
    {
        plugin->load();
    }
    return true;
}

PluginInstance *PluginManager::load(const std::string &name, int &errCode)
{
    std::map<std::string, DynamicLib *>::iterator iter = mPluginLibs.find(name);
    if (iter == mPluginLibs.end())
    {
        DynamicLib *lib = new DynamicLib;
        if (lib != nullptr)
        {
            lib->loadLib(name.c_str());
            mPluginLibs.insert(make_pair(name, lib));
            START_PLUGIN_FUN fun = (START_PLUGIN_FUN) lib->getSymbolAddress("StartPlugin");
            if (fun != nullptr)
            {
                PluginInstance *plugin = fun();
                errCode                = 1;
                return plugin;
            }
        }
    }
    else
    {
        for (int i = 0; i < mPlugins.size(); ++i)
        {
            if (name == mPlugins[i]->getDisplayName())
            {
                errCode = 1;
                return mPlugins[i];
            }
        }
    }
    errCode = 0;
    return nullptr;
}

bool PluginManager::loadPlugin(PluginInstance *plugin)
{
    mPlugins.push_back(plugin);
    return true;
}

bool PluginManager::unLoadAll()
{
    return true;
}

bool PluginManager::unLoad(const string &name)
{
    std::map<std::string, DynamicLib *>::iterator iter = mPluginLibs.begin();
    for (; iter != mPluginLibs.end(); ++iter)
    {
        DynamicLib *lib = iter->second;
        if (lib == nullptr)
        {
            continue;
        }
        if (lib->getName() == name)
        {
            STOP_PLUGIN_FUN fun = (STOP_PLUGIN_FUN) lib->getSymbolAddress("StopPlugin");
            if (fun != nullptr)
            {
                fun();
            }
            lib->freeLib();
            delete lib;
            mPluginLibs.erase(iter);
            return true;
        }
    }
    return false;
}

bool PluginManager::unLoadPlugin(PluginInstance *plugin)
{
    std::vector<PluginInstance *>::iterator iter = mPlugins.begin();
    for (; iter != mPlugins.end(); ++iter)
    {
        if (plugin == *iter)
        {
            mPlugins.erase(iter);
            return true;
        }
    }
    return false;
}

std::vector<PluginInstance *> PluginManager::getAllPlugins()
{
    return mPlugins;
}

PluginManager::PluginManager()
{
    mPluginLibs.clear();
    mPlugins.clear();
}

PluginManager::~PluginManager()
{
}

