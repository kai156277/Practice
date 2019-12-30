#include "DynamicLib.h"

#include <Windows.h>
#include <iostream>
#include <string>

DynamicLib::DynamicLib()
    : mDynlib(nullptr)
    , mLibName(nullptr)
{
}

DynamicLib::~DynamicLib()
{
    if (mDynlib)
        freeLib();

    if (!mLibName.empty())
        mLibName.clear();
}

std::string DynamicLib::getName() const
{
    return mLibName;
}

bool DynamicLib::loadLib(const std::string &strLibName)
{
    std::string strName = strLibName + (".dll");

    mDynlib = LoadLibraryA(strName.c_str());
    if (mDynlib)
    {
        mLibName = strdup(strLibName.c_str());
        return true;
    }

    std::cout << GetLastError();
    return false;
}

void *DynamicLib::getSymbolAddress(const std::string &strSymbolName) const
{
    if (mDynlib)
    {
        return (void *) GetProcAddress((HMODULE) mDynlib, strSymbolName.c_str());
    }
    return nullptr;
}

void DynamicLib::freeLib()
{
    if (mDynlib)
    {
        FreeLibrary((HMODULE) mDynlib);
        mDynlib = nullptr;
    }

    if (!mLibName.empty())
        mLibName.clear();
}
