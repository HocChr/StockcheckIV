#include "tableViewModel.h"

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// ----------------------------------------------------------------------------
// --- Sortierfunktionen für die Spalten --------------------------------------
// ----------------------------------------------------------------------------

struct orderByDividend
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.DividendGrowthFiveYears() < ent2.DividendGrowthFiveYears());
    }
};

struct orderByDividendDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.DividendGrowthFiveYears() > ent2.DividendGrowthFiveYears());
    }
};

struct orderByName
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.Name() < ent2.Name());
    }
};

struct orderByNameDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.Name() > ent2.Name());
    }
};

struct orderByYear
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        if(ent1.GetYearData().size() < 1 || ent2.GetYearData().size() < 1)
            return false;
        return (ent1.GetYearData().at(ent1.GetYearData().size()-1).Year <
                ent2.GetYearData().at(ent2.GetYearData().size()-1).Year);
    }
};

struct orderByYearDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        if(ent1.GetYearData().size() < 1 || ent2.GetYearData().size() < 1)
            return false;
        return (ent1.GetYearData().at(ent1.GetYearData().size()-1).Year >
                ent2.GetYearData().at(ent2.GetYearData().size()-1).Year);
    }
};

struct orderByRate
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.Rating() < ent2.Rating());
    }
};

struct orderByRateDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.Rating() > ent2.Rating());
    }
};

struct orderByRank
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.Percentage() < ent2.Percentage());
    }
};

struct orderByRankDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        if(std::abs(ent1.Percentage() - ent2.Percentage()) < 0.01)
            return ent1.DividendGrowthFiveYears() > ent2.DividendGrowthFiveYears();
        return (ent1.Percentage() > ent2.Percentage());
    }
};

struct orderByRevenueGrowth
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.RevenueGrowthFiveYears() < ent2.RevenueGrowthFiveYears());
    }
};

struct orderByRevenueGrowthDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.RevenueGrowthFiveYears() > ent2.RevenueGrowthFiveYears());
    }
};

struct orderByEarningGrowth
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.EarningGrowthFiveYears() < ent2.EarningGrowthFiveYears());
    }
};

struct orderByEarningGrowthDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.EarningGrowthFiveYears() > ent2.EarningGrowthFiveYears());
    }
};

struct orderByDividendGrowth
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.DividendGrowthFiveYears() < ent2.DividendGrowthFiveYears());
    }
};

struct orderByDividendGrowthDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.DividendGrowthFiveYears() > ent2.DividendGrowthFiveYears());
    }
};

struct orderByPayoutRatio
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.PayoutRatio() < ent2.PayoutRatio());
    }
};

struct orderByPayoutRatioDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.PayoutRatio() > ent2.PayoutRatio());
    }
};

struct orderByRemark
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.GetRemarks() < ent2.GetRemarks());
    }
};

struct orderByRemarkDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        auto& lhs = ent1.GetRemarks();
        auto& rhs = ent2.GetRemarks();

        return (lhs > rhs);
    }
};

// -------------------------------------------------------
// --- Test Database -------------------------------------
// -------------------------------------------------------

class TestDatabase : public IDatabase
{
public:
    std::vector<StockEntity> GetStocks() override final;

private:
    StockEntity GetStockBayer();
};

//----------------------------------------------------------
//--- TableModel -------------------------------------------
//----------------------------------------------------------

TableModel::TableModel() : QAbstractTableModel (nullptr)
{
    onActualize();
}

int TableModel::rowCount(const QModelIndex &) const
{
    return static_cast<int>(_data.size());
}

int TableModel::columnCount(const QModelIndex &) const
{
    return 11;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
    case Role::Name:
        return QString::fromStdString(_data[static_cast<size_t>(index.row())].Name());
    case Role::Year :
    {
        size_t end = _data[static_cast<size_t>(index.row())].GetYearData().size() - 1;
        bool hasAtLeastOneYear = end > 0;
        int lastYear = hasAtLeastOneYear ? _data[static_cast<size_t>(index.row())].GetYearData().at(end).Year: 0;
        return  QString::number(lastYear);
    }
    case Role::Rate:
    {
        switch (_data[static_cast<size_t>(index.row())].Rating())
        {
        case StockEntity::Rate::A:
            return  QString("A");
        case StockEntity::Rate::B:
            return  QString("B");
        case StockEntity::Rate::C:
            return  QString("C");
        }
    }
    case Role::Percentage:
        return QString::number(_data[static_cast<size_t>(index.row())].Percentage(), 'f', 0);
    case Role::RevenueGrowth:
        return QString::number(_data[static_cast<size_t>(index.row())].RevenueGrowthFiveYears(), 'f', 2);
    case Role::EarningGrowth:
        return QString::number(_data[static_cast<size_t>(index.row())].EarningGrowthFiveYears(), 'f', 1);
    case Role::DividendGrowth:
        return QString::number(_data[static_cast<size_t>(index.row())].DividendGrowthFiveYears(), 'f', 1);
    case Role::PayoutRatio:
        return QString::number(_data[static_cast<size_t>(index.row())].PayoutRatio(), 'f', 0);
     case Role::Remark:
        return QString::fromStdString(_data[static_cast<size_t>(index.row())].GetRemarks());
    default:
        return QString("Not supposed to happen");
    }
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Role::Name] = "name";
    roles[Role::Year] = "year";
    roles[Role::Rate] = "rate";
    roles[Role::Percentage] = "percentage";
    roles[Role::RevenueGrowth] = "revenueGrowth";
    roles[Role::EarningGrowth] = "earningGrowth";
    roles[Role::DividendGrowth] = "dividendGrowth";
    roles[Role::PayoutRatio] = "payoutRatio";
    roles[Role::Remark] = "remark";
    return roles;
}

void TableModel::sort(int column, Qt::SortOrder order)
{
    bool ascending = order == Qt::SortOrder::AscendingOrder;

    //std::sort(_data.begin(), _data.end(), orderByDividend());
    if(ascending)
    {
        switch (column)
        {
        case 0:
            std::sort(_data.begin(), _data.end(), orderByName());
            break;
        case 1:
            std::sort(_data.begin(), _data.end(), orderByYear());
            break;
        case 2:
            std::sort(_data.begin(), _data.end(), orderByRate());
            break;
        case 3:
            std::sort(_data.begin(), _data.end(), orderByRank());
            break;
        case 4:
            std::sort(_data.begin(), _data.end(), orderByRevenueGrowth());
            break;
        case 5:
            std::sort(_data.begin(), _data.end(), orderByEarningGrowth());
            break;
        case 6:
            std::sort(_data.begin(), _data.end(), orderByDividendGrowth());
            break;
        case 7:
            std::sort(_data.begin(), _data.end(), orderByPayoutRatio());
            break;
        case 8:
            std::sort(_data.begin(), _data.end(), orderByRemark());
            break;
        default:
            break;
        }
    }
    else {
        switch (column)
        {
        case 0:
            std::sort(_data.begin(), _data.end(), orderByNameDescending());
            break;
        case 1:
            std::sort(_data.begin(), _data.end(), orderByYearDescending());
            break;
        case 2:
            std::sort(_data.begin(), _data.end(), orderByRateDescending());
            break;
        case 3:
            std::sort(_data.begin(), _data.end(), orderByRankDescending());
            break;
        case 4:
            std::sort(_data.begin(), _data.end(), orderByRevenueGrowthDescending());
            break;
        case 5:
            std::sort(_data.begin(), _data.end(), orderByEarningGrowthDescending());
            break;
        case 6:
            std::sort(_data.begin(), _data.end(), orderByDividendGrowthDescending());
            break;
        case 7:
            std::sort(_data.begin(), _data.end(), orderByPayoutRatioDescending());
            break;
        case 8:
            std::sort(_data.begin(), _data.end(), orderByRemarkDescending());
            break;
        default:
            break;
        }
    }
    emit layoutChanged();
}

void TableModel::getLineSeriesPerShare(int rowId,
                                       QLineSeries* lineSeriesEarnings,
                                       QLineSeries* scatterSeriesEarnings,
                                       QLineSeries* lineSeriesDividends,
                                       QLineSeries* scatterSeriesDividends,
                                       QValueAxis* xAxis,
                                       QValueAxis* yAxis)
{
    if(rowId == -1) return;

    size_t id = static_cast<size_t>(rowId);
    if(_data.size() < id)
    {
        return;
    }

    double xMin{1.0e9}, yMin{1.0e9}, xMax{-1.0e9}, yMax{-1.0e9};
    for(const auto& yearDataSet : _data.at(id).GetYearData())
    {
        // Bestimme die x-y-Spanne für die Achsen
        if(yearDataSet.Year < xMin)
        {
            xMin = yearDataSet.Year;
        }

        if(yearDataSet.Year > xMax)
        {
            xMax = yearDataSet.Year;
        }

        if(std::min(yearDataSet.Dividend, yearDataSet.Earnings) < yMin)
        {
            yMin = std::min(yearDataSet.Dividend, yearDataSet.Earnings)*1.01;
        }

        if(std::max(yearDataSet.Dividend, yearDataSet.Earnings) > yMax)
        {
            yMax = std::max(yearDataSet.Dividend, yearDataSet.Earnings)*1.01;
        }

        // erzeuge die Graphen
        double m = _data.at(id).EarningRegression().m;
        double n = _data.at(id).EarningRegression().n;

        if (yearDataSet.Year >= _data.at(id).EarningRegression().startYear)
            lineSeriesEarnings->append(yearDataSet.Year, m*yearDataSet.Year + n);
        if (std::abs(yearDataSet.Earnings) > 0.1)
            scatterSeriesEarnings->append(yearDataSet.Year, yearDataSet.Earnings);

        m = _data.at(id).DividendRegresssion().m;
        n = _data.at(id).DividendRegresssion().n;

        if (yearDataSet.Year >= _data.at(id).DividendRegresssion().startYear)
            lineSeriesDividends->append(yearDataSet.Year, yearDataSet.Year*m + n);
        if (std::abs(yearDataSet.Dividend) > 0.1)
            scatterSeriesDividends->append(yearDataSet.Year, yearDataSet.Dividend);

        // setze die x-y-Spanne auf die Achsen
        xAxis->setMin(xMin);
        xAxis->setMax(xMax);
        yAxis->setMin(yMin);
        yAxis->setMax(yMax);
    }
}

void TableModel::getLineSeriesRevenue(int rowId,
                                      QLineSeries *lineSeriesRevenues,
                                      QLineSeries *scatterSerieRevenues,
                                      QValueAxis *yAxis)
{
    if(rowId == -1) return;

    size_t id = static_cast<size_t>(rowId);
    if(_data.size() < id)
    {
        return;
    }

    double yMin{1.0e9}, yMax{-1.0e9};
    for(const auto& yearDataSet : _data.at(id).GetYearData())
    {
        yMin = std::min(yMin, yearDataSet.Revenue)*1.01;
        yMax = std::max(yMax, yearDataSet.Revenue)*1.01;

        double m = _data.at(id).RevenueRegression().m;
        double n = _data.at(id).RevenueRegression().n;

        if (yearDataSet.Year >= _data.at(id).RevenueRegression().startYear)
            lineSeriesRevenues->append(yearDataSet.Year, m*yearDataSet.Year + n);

        if (std::abs(yearDataSet.Revenue) > 0.1)
            scatterSerieRevenues->append(yearDataSet.Year, yearDataSet.Revenue);
    }

    // setze die x-y-Spanne auf die Achsen
    yAxis->setMin(yMin);
    yAxis->setMax(yMax);
}

QString TableModel::getTableName(int rowId)
{
    if(rowId == -1) return QString("Krass Error: getTableName");

    size_t id = static_cast<size_t>(rowId);
    if(_data.size() < id)
    {
        QString("Krass Error: getTableName");
    }
    return QString(_data.at(id).Name().c_str());
}

void TableModel::onDatabaseChanged(QString databaseNameNew)
{
    databaseChanged(databaseNameNew);

    auto config = ConfigurationHandler::getConfiguration();
    config.CURRENT_DATABASE = databaseNameNew.toStdString();
    ConfigurationHandler::setConfiguration(config);
}

void TableModel::onActualize()
{
    ConfigurationHandler::Configuration config = ConfigurationHandler::getConfiguration();
    _dataBase = make_unique<SqliteAccess>();
    databaseChanged(config.CURRENT_DATABASE.c_str());

    emit layoutChanged();
}

void TableModel::databaseChanged(QString databaseNameNew)
{
    std::experimental::filesystem::path path = std::experimental::filesystem::current_path();
    path.append("database");
    path.append(databaseNameNew.toStdString() + ".db");

    _dataBase->setDatabasePath(path);

    Stockcheck stockcheck(_dataBase.get());
    _data = stockcheck.GetStocksList();
    std::sort(_data.begin(), _data.end(), orderByRankDescending());

    emit layoutChanged();
}

// -------------------------------------------------------
// --- Implementierung Test Database ---------------------
// -------------------------------------------------------

std::vector<StockEntity> TestDatabase::GetStocks()
{
    std::vector<StockEntity> stockList;
    stockList.push_back(GetStockBayer());

    return stockList;
}

StockEntity TestDatabase::GetStockBayer()
{
    StockEntity entity("Bayer");

    auto yearData = entity.GetYearData();
    yearData.push_back(StockEntity::YearDataSet(2007, 1000, 2.91, 1.35));
    yearData.push_back(StockEntity::YearDataSet(2008, 1000, 2.2, 1.4));
    yearData.push_back(StockEntity::YearDataSet(2009, 1000, 1.7, 1.4));
    yearData.push_back(StockEntity::YearDataSet(2010, 1000, 1.57, 1.5));
    yearData.push_back(StockEntity::YearDataSet(2011, 1000, 2.99, 1.65));
    yearData.push_back(StockEntity::YearDataSet(2012, 1000, 2.96, 1.9));
    yearData.push_back(StockEntity::YearDataSet(2013, 1000, 3.86, 2.1));
    yearData.push_back(StockEntity::YearDataSet(2014, 1000, 5.59, 2.25));
    yearData.push_back(StockEntity::YearDataSet(2015, 1000, 6.82, 2.5));
    yearData.push_back(StockEntity::YearDataSet(2016, 1001, 6.67, 2.7));
    yearData.push_back(StockEntity::YearDataSet(2017, 1001, 6.64, 2.8));
    yearData.push_back(StockEntity::YearDataSet(2018, 1001, 5.94, 2.8));
    yearData.push_back(StockEntity::YearDataSet(2019, 1001, 6.4, 2.8));
    entity.SetYearData(yearData);

    return entity;
}
