#ifndef BASEPLUGININTERFACE_H
#define BASEPLUGININTERFACE_H

#include <QString>

class BasePluginInterface
{
public:
    BasePluginInterface() {}
    virtual QString name() const        = 0;
    virtual QString description() const = 0;
};

#endif   // BASEPLUGININTERFACE_H
