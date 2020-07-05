#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

#include "stockcheck/stockcheck.h"
#include "stockcheck/idatabase.h"
#include "tableViewModel.h"

class TestDatabase : public IDatabase
{
public:

    std::vector<StockEntity> GetStocks() override
    {
        std::vector<StockEntity> stockList;
        stockList.push_back(GetStockBayer());

        return stockList;
    }

private:

    StockEntity GetStockBayer()
    {
        StockEntity entity("Bayer");

        auto yearData = entity.GetYearData();
        yearData.push_back(StockEntity::YearDataSet(2007, 2.91, 1.35));
        yearData.push_back(StockEntity::YearDataSet(2008, 2.2, 1.4));
        yearData.push_back(StockEntity::YearDataSet(2009, 1.7, 1.4));
        yearData.push_back(StockEntity::YearDataSet(2010, 1.57, 1.5));
        yearData.push_back(StockEntity::YearDataSet(2011, 2.99, 1.65));
        yearData.push_back(StockEntity::YearDataSet(2012, 2.96, 1.9));
        yearData.push_back(StockEntity::YearDataSet(2013, 3.86, 2.1));
        yearData.push_back(StockEntity::YearDataSet(2014, 5.59, 2.25));
        yearData.push_back(StockEntity::YearDataSet(2015, 6.82, 2.5));
        yearData.push_back(StockEntity::YearDataSet(2016, 6.67, 2.7));
        yearData.push_back(StockEntity::YearDataSet(2017, 6.64, 2.8));
        yearData.push_back(StockEntity::YearDataSet(2018, 5.94, 2.8));
        yearData.push_back(StockEntity::YearDataSet(2019, 6.4, 2.8));
        entity.SetYearData(yearData);

        return entity;
    }
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    TableViewModel tableViewmodel;

    qmlRegisterType<TableModel>("TableModel", 0, 1, "TableModel");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //engine.rootContext()->setContextProperty("tableViewModel", &tableViewmodel);



    // test
    std::unique_ptr<IDatabase> db = std::make_unique<TestDatabase>();

    Stockcheck stockcheck(db.get());
    std::vector<StockEntity> stocks = stockcheck.GetStocksList();

    return app.exec();
}
