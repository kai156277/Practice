#pragma once
#include "DataSource.h"
#include <QVector>

namespace pos {

class DataManager
{
public:
    enum ErrorType
    {
        kOk = 0,
        kEmptyInput
    };
    DataManager();

    ErrorType registerDataSource(DataSource *src);

private:
    QVector<DataSourceSptr> data_sources_;
};

}   // namespace pos
