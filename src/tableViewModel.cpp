#include "tableViewModel.h"

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// -------------------------------------------------------
// --- Test Database -------------------------------------
// -------------------------------------------------------
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

//----------------------------------------------------------
//--- TableModel -------------------------------------------
//----------------------------------------------------------

TableModel::TableModel() : QAbstractTableModel (nullptr)
{
    // test
    std::unique_ptr<IDatabase> db = make_unique<TestDatabase>();

    Stockcheck stockcheck(db.get());
    _data = stockcheck.GetStocksList();
}

int TableModel::rowCount(const QModelIndex &) const
{
    return _data.size();
}

int TableModel::columnCount(const QModelIndex &) const
{
    return 10;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    // Check DisplayRole
    //if(role != Qt::DisplayRole)
    //{
    //    return QVariant();
    //}

    switch(role)
    {
    case 1000:
        return QString::fromStdString(_data[index.row()].Name());
    case 1001:
        return  QString("1999");
    case 1002:
    {
        switch (_data[index.row()].Rating())
        {
        case StockEntity::Rate::A:
            return  QString("A");
        case StockEntity::Rate::B:
            return  QString("B");
        case StockEntity::Rate::C:
            return  QString("C");
        }
    }
    case 1003:
        return QString::number(_data[index.row()].Percentage(), 'f', 1);
    case 1004:
        return QString::number(_data[index.row()].EarningCorrelation(), 'f', 1);
    case 1005:
        return QString::number(_data[index.row()].EarningGrowthThreeYears(), 'f', 1);
    case 1006:
        return QString::number(_data[index.row()].DividendGrowthThreeYears(), 'f', 1);
    case 1007:
        return QString::number(_data[index.row()].PayoutRatio(), 'f', 1);
    case 1008:
        return QString::fromStdString(_data[index.row()].GetRemarks());
    default:
        qDebug() << "Not supposed to happen";
        return QVariant();
    }



    //QVariant value;
    //
    //if (index.isValid()) {
    //    if (role < Qt::UserRole) {
    //        value = "nu";
    //    } else {
    //        value = "Inavlid";
    //    }
    //}
    //return value;
    //switch (role) {
    //    case Qt::DisplayRole:
    //    {
    //
    //        QString colname = roleNames()[index.column()];
    //        return QString("%1, %2").arg(colname).arg(index.row());
    //    }
    //    default:
    //        break;
    //}
    //
    //return QVariant();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[1000] = "name";
    roles[1001] = "year";
    roles[1002] = "rate";
    roles[1003] = "percentage";
    roles[1004] = "earningCorrelation";
    roles[1005] = "earningGrowth";
    roles[1006] = "dividendGrowth";
    roles[1007] = "payoutRatio";
    roles[1008] = "remark";
    return roles;
}
