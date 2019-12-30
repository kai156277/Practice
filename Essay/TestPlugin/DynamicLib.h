#ifndef DYNAMICLIB_H
#define DYNAMICLIB_H

#include "Export.h"

#include <string>

class DynamicLib
{
public:
    DynamicLib();
    ~DynamicLib();

    std::string getName() const;

    bool  loadLib(const std::string &strLibName);
    void *getSymbolAddress(const std::string &strSymbolName) const;
    void  freeLib();

private:
    void *      mDynlib;    // 动态库句柄
    std::string mLibName;   // 动态库名字
};

#endif   // DYNAMICLIB_H
