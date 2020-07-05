#ifndef IDATABASE_H
#define IDATABASE_H

#include "stockentity.h"
#include <vector>

class IDatabase
{
public:

    virtual std::vector<StockEntity> GetStocks() = 0;
};

#endif // IDATABASE_H
