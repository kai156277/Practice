#pragma once
#include "DataSource.h"
#include <QString>

namespace pos {
class DataSourceFactory
{
public:
    DataSourceFactory();

    DataSource *getDataSource(const QString &src_name);
};

}   // namespace pos
