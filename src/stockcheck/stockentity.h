#ifndef STOCKENTITY_H
#define STOCKENTITY_H

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

// Datensatz, der eine Aktie repräsentiert
class StockEntity
{

public:

    // Datentyp, der den Datensatz eines Jahres repräsentiert
    class YearDataSet
    {
    public:
        double Dividend{0.0};
        double Earnings{0.0};
        int Year{0};

        YearDataSet(int year, double earning, double dividend) :
            Dividend{dividend}, Earnings{earning}, Year{year} 
        {
        }

        bool operator <(const YearDataSet& other)
        {
            return this->Year < other.Year;
        }
    };

    enum class Rate
    {
        A = 0,
        B,
        C
    };

    enum class StockType
    {
        None = 0,
        DivididendStock,
        GrowthStock
    };

    StockEntity(std::string name) : _name(std::move(name))
    {
    }

    //StockEntity()= delete;

    //StockEntity(const StockEntity& other)
    //{
    //    this->_name = other._name;
    //}
    //
    //StockEntity(StockEntity&& other)
    //{
    //    std::move(other);
    //}
    //
    //StockEntity& operator=(const StockEntity& other)
    //{
    //    this->_name = other._name;
    //    return *this;
    //}
    //
    //StockEntity& operator=(StockEntity&& other)
    //{
    //    std::move(other);
    //    return *this;
    //}

    Rate Rating() const
    {
        return _rating;
    }

    void SetRating(Rate value)
    {
        _rating = value;
    }

    StockType Type() const
    {
        return _type;
    }

    void SetStockType(StockType value)
    {
        _type = value;
    }

    std::string Name() const
    {
        return _name;
    }

    void SetName(std::string value)
    {
        _name = value;
    }

    double Percentage() const
    {
        return _percentage;
    }

    void SetPercentag(double value)
    {
        _percentage = value;
    }

    double EarningCorrelation() const
    {
        return _earningCorrelation;
    }

    void SetEarningCorrelation(double value)
    {
        _earningCorrelation = value;
    }

    double EarningGrowthThreeYears() const
    {
        return _earningGrowth;
    }

    void SetEarningGrowthThreeYears(double value)
    {
        _earningGrowth = value;
    }

    double DividendGrowthThreeYears() const
    {
        return _dividendGrowth;
    }

    void SetDividendGrowthThreeYears(double value)
    {
        _dividendGrowth = value;
    }

    double PayoutRatio() const
    {
        return _payoutRatio;
    }

    void SetPayoutRatio(double value)
    {
        _payoutRatio = value;
    }

    int NumYearsDividendNotReduced() const
    {
        return _numYearsDividendNotReduced;
    }

    void SetNumYearsDividendNotReduced(int value)
    {
        _numYearsDividendNotReduced = value;
    }

    const std::vector<YearDataSet>& GetYearData() const
    {
        return _yearData;
    }

    void SetYearData(const std::vector<YearDataSet>& value)
    {
        _yearData = value;
        sort(_yearData.begin(), _yearData.end());
    }

    void AddRemark(const std::string& remark)
    {
        _remarks += remark + "; ";
    }

    const std::string& GetRemarks() const
    {
        return _remarks;
    }

 private:
     std::vector<StockEntity::YearDataSet> _yearData;
     std::string _name = "";
     std::string _remarks = "";
     Rate _rating = Rate::C;
     StockType _type = StockType::None;

     double _percentage{0.0};
     double _earningCorrelation{0.0};
     double _earningGrowth{0.0};
     double _dividendGrowth{0.0};
     double _payoutRatio{0.0};

     int _numYearsDividendNotReduced{0};
};

#endif // STOCKENTITY_H
