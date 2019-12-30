#include "DataManager.h"

namespace pos {

DataManager::DataManager()
{
}

DataManager::ErrorType DataManager::registerDataSource(DataSource *src)
{
    if (src == nullptr)
    {
        return kEmptyInput;
    }

    DataSourceSptr src_sptr(src);

    for (const auto &ptr : data_sources_)
    {
    }
}

}   // namespace pos
