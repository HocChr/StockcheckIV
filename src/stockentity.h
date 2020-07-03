#ifndef STOCKENTITY_H
#define STOCKENTITY_H

#include <list>
#include <string>

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

        bool operator <(const YearDataSet& other)
        {
            return this->Year < other.Year;
        }
    };

    enum Rate
    {
        A,
        B,
        C
    };

    StockEntity(std::string name)
    {
        _name = name;
    }

    Rate Rating() const
    {
        return _rating;
    }

    void SetRating(Rate value)
    {
        _rating = value;
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

    std::list<YearDataSet> GetYearData() const
    {
        return _yearData;
    }

    void SetYearData(const std::list<YearDataSet>& value)
    {
        _yearData = value;
        _yearData.sort();
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
     std::list<StockEntity::YearDataSet> _yearData;
     std::string _name;
     std::string _remarks = "";
     Rate _rating = Rate::C;

     double _percentage{0.0};
     double _earningCorrelation{0.0};
     double _earningGrowth{0.0};
     double _dividendGrowth{0.0};
     double _payoutRatio{0.0};

     int _numYearsDividendNotReduced{0};
};

#endif // STOCKENTITY_H
