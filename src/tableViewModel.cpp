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
        return (ent1.DividendGrowthThreeYears() < ent2.DividendGrowthThreeYears());
    }
};

struct orderByDividendDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.DividendGrowthThreeYears() > ent2.DividendGrowthThreeYears());
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
        return (ent1.Percentage() > ent2.Percentage());
    }
};

struct orderByEarningCorrelation
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.EarningCorrelation() < ent2.EarningCorrelation());
    }
};

struct orderByEarningCorrelationDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.EarningCorrelation() > ent2.EarningCorrelation());
    }
};

struct orderByEarningGrowth
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.EarningGrowthThreeYears() < ent2.EarningGrowthThreeYears());
    }
};

struct orderByEarningGrowthDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.EarningGrowthThreeYears() > ent2.EarningGrowthThreeYears());
    }
};

struct orderByDividendGrowth
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.DividendGrowthThreeYears() < ent2.DividendGrowthThreeYears());
    }
};

struct orderByDividendGrowthDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.DividendGrowthThreeYears() > ent2.DividendGrowthThreeYears());
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

struct orderByType
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        return (ent1.Type() < ent2.Type());
    }
};

struct orderByTypeDescending
{
    inline bool operator() (const StockEntity& ent1, const StockEntity& ent2)
    {
        auto lhs = ent1.Type();
        auto rhs = ent2.Type();

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
    case Role::RevenueCorrelation:
        return QString::number(_data[static_cast<size_t>(index.row())].RevenueCorrelation(), 'f', 2);
    case Role::EarningCorrelation:
        return QString::number(_data[static_cast<size_t>(index.row())].EarningCorrelation(), 'f', 2);
    case Role::EarningGrowth:
        return QString::number(_data[static_cast<size_t>(index.row())].EarningGrowthThreeYears(), 'f', 1);
    case Role::DividendGrowth:
        return QString::number(_data[static_cast<size_t>(index.row())].DividendGrowthThreeYears(), 'f', 1);
    case Role::PayoutRatio:
        return QString::number(_data[static_cast<size_t>(index.row())].PayoutRatio(), 'f', 0);
    case Role::StockType:
    {
        switch (_data[static_cast<size_t>(index.row())].Type())
        {
        case StockEntity::StockType::None:
            return QString("Nix");
        case StockEntity::StockType::GrowthStock:
            return QString("Wachstum");
        case StockEntity::StockType::DivididendStock:
            return QString("Dividende");
        }
    }
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
    roles[Role::RevenueCorrelation] = "revenueCorrelation";
    roles[Role::EarningCorrelation] = "earningCorrelation";
    roles[Role::EarningGrowth] = "earningGrowth";
    roles[Role::DividendGrowth] = "dividendGrowth";
    roles[Role::PayoutRatio] = "payoutRatio";
    roles[Role::StockType] = "stockType";
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
            //std::sort(_data.begin(), _data.end(), orderByEarningCorrelation());
            break;
        case 5:
            std::sort(_data.begin(), _data.end(), orderByEarningCorrelation());
            break;
        case 6:
            std::sort(_data.begin(), _data.end(), orderByEarningGrowth());
            break;
        case 7:
            std::sort(_data.begin(), _data.end(), orderByDividendGrowth());
            break;
        case 8:
            std::sort(_data.begin(), _data.end(), orderByPayoutRatio());
            break;
        case 9:
            std::sort(_data.begin(), _data.end(), orderByType());
            break;
        case 10:
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
            //std::sort(_data.begin(), _data.end(), orderByEarningCorrelationDescending());
            break;
        case 5:
            std::sort(_data.begin(), _data.end(), orderByEarningCorrelationDescending());
            break;
        case 6:
            std::sort(_data.begin(), _data.end(), orderByEarningGrowthDescending());
            break;
        case 7:
            std::sort(_data.begin(), _data.end(), orderByDividendGrowthDescending());
            break;
        case 8:
            std::sort(_data.begin(), _data.end(), orderByPayoutRatioDescending());
            break;
        case 9:
            std::sort(_data.begin(), _data.end(), orderByTypeDescending());
            break;
        case 10:
            std::sort(_data.begin(), _data.end(), orderByRemarkDescending());
            break;
        default:
            break;
        }
    }
    emit layoutChanged();
}

void TableModel::getLineSeries(int rowId,
                               QLineSeries* lineSeriesEarnings,
                               QLineSeries* lineSeriesDividends,
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
            yMin = std::min(yearDataSet.Dividend, yearDataSet.Earnings);
        }

        if(std::max(yearDataSet.Dividend, yearDataSet.Earnings) > yMax)
        {
            yMax = std::max(yearDataSet.Dividend, yearDataSet.Earnings);
        }

        // erzeuge die Graphen
        lineSeriesEarnings->append(yearDataSet.Year, yearDataSet.Earnings);
        lineSeriesDividends->append(yearDataSet.Year, yearDataSet.Dividend);

        // setze die x-y-Spanne auf die Achsen
        xAxis->setMin(xMin);
        xAxis->setMax(xMax);
        yAxis->setMin(yMin);
        yAxis->setMax(yMax);
    }
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
