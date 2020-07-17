#ifndef SQLITEACCESS_H
#define SQLITEACCESS_H

#include "../stockcheck/idatabase.h"

#include <vector>
#include <experimental/filesystem>

class SqliteAccess : public IDatabase
{
public:
    void setDatabasePath(std::experimental::filesystem::path databasePath);

    std::vector<StockEntity> GetStocks() override final;

private:
    std::experimental::filesystem::path _databasePath;
};

#endif // SQLITEACCESS_H
