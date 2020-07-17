#include "sqliteAccess.h"
#include "sqlite3.h"

#include <sstream>
#include <string.h>

//static int callback(void *data, int argc, char **argv, char **azColName){
//    int i;
//    fprintf(stderr, "%s: ", (const char*)data);
//
//    for(i = 0; i<argc; i++)
//    {
//        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//
//    printf("\n");
//    return 0;
//}

static int callbackGetTableNames(void *data, int argc, char **argv, char **){

    std::vector<std::string>* tableNames = static_cast<std::vector<std::string>*>(data);

    for(int i = 0; i<argc; i++)
    {
        tableNames->push_back(argv[i] ? argv[i] : "NULL");
    }

    return 0;
}

static int callbackFillEntities(void *data, int argc, char **argv, char **azColName){

    StockEntity* entity = static_cast<StockEntity*>(data);

    int year{0};
    double earning{0.0}, dividend{0.0};
    auto yearData = entity->GetYearData();
    for(int i = 0; i<argc; i++)
    {

        if(strcmp ("year", azColName[i]) == 0)
        {
            year = atoi(argv[i]);
        }
        else if(strcmp ("earning_per_share", azColName[i]) == 0)
        {
            earning = atof(argv[i]);
        }
        else if(strcmp ("div_per_share", azColName[i]) == 0)
        {
            dividend = atof(argv[i]);
        }
        else
        {
            continue;
        }
    }
    yearData.push_back(StockEntity::YearDataSet(year, earning, dividend));
    entity->SetYearData(yearData);

    return 0;
}

// ----------------------------------------------------------------------------
// ---  SqliteDatabaseHandler -------------------------------------------------
// ----------------------------------------------------------------------------

class SqliteDatabaseHandler
{
public:
    SqliteDatabaseHandler() = delete;

    SqliteDatabaseHandler(std::experimental::filesystem::path databasePath)
    {
        sqlite3_open(databasePath.string().c_str(), &db);
    }

    sqlite3* getDatabaseHandler()
    {
        return db;
    }

    ~SqliteDatabaseHandler()
    {
       sqlite3_close(db);
    }

private:
    sqlite3 *db;
};

//-----------------------------------------------------------------------------
//--- Helpers Functions -------------------------------------------------------
//-----------------------------------------------------------------------------

static std::vector<std::string> getTableNames(SqliteDatabaseHandler& dbHandler)
{
    std::vector<std::string> tableNames;

    std::string sql = "SELECT name FROM sqlite_master WHERE type ='table' AND name NOT LIKE 'sqlite_%';";
    sqlite3_exec(dbHandler.getDatabaseHandler(),
                 sql.c_str(),
                 callbackGetTableNames, static_cast<void*>(&tableNames), nullptr);

    return tableNames;
}

static StockEntity getStock(SqliteDatabaseHandler& dbHandler, std::string name)
{
    StockEntity entity(name);

    std::string sql = "SELECT * FROM [" + name +"];";
    sqlite3_exec(dbHandler.getDatabaseHandler(),
                 sql.c_str(),
                 callbackFillEntities,
                 static_cast<void*>(&entity), nullptr);

    return entity;
}

// ----------------------------------------------------------------------------
// --- SqliteAccess -----------------------------------------------------------
// ----------------------------------------------------------------------------

void SqliteAccess::setDatabasePath(std::experimental::filesystem::__cxx11::path databasePath)
{
    _databasePath = databasePath;
}

std::vector<StockEntity> SqliteAccess::GetStocks()
{
    std::vector<StockEntity> stocks;
    SqliteDatabaseHandler dbHandler(_databasePath);
    std::vector<std::string> tablesNames = getTableNames(dbHandler);

    for(const auto& stock : tablesNames)
    {
        stocks.push_back(getStock(dbHandler, stock));
    }

    return stocks;
}
