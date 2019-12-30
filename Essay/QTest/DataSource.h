#pragma once
#include <memory>

namespace pos {

class DataSource
{
public:
    DataSource();
};

typedef std::shared_ptr<pos::DataSource> DataSourceSptr;
}   // namespace pos
