#ifndef IDATABASE_H
#define IDATABASE_H

#include "stockentity.h"
#include <list>

class IDatabase
{
    virtual std::list<StockEntity> GetStocks() = 0;
};

#endif // IDATABASE_H
