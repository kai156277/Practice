#include "DataSourceFactory.h"

namespace pos {

DataSourceFactory::DataSourceFactory()
{
}

DataSource *DataSourceFactory::getDataSource(const QString &src_name)
{
    return new DataSource();
}

}   // namespace pos
