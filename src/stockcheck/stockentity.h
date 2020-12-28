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
        double Revenue{0.0};
        double Dividend{0.0};
        double Earnings{0.0};
        int Year{0};

        YearDataSet(int year, double revenue, double earning, double dividend) :
            Revenue{revenue}, Dividend{dividend}, Earnings{earning}, Year{year}
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

    StockEntity(std::string name) : _name(std::move(name))
    {
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

    int RevenueStability() const
    {
        return _revenueStability;
    }

    void SetRevenueStability(int value)
    {
        _revenueStability = value;
    }

    double RevenueGrowthThreeYears() const
    {
        return _revenueGrowth;
    }

    void SetRevenueGrowthThreeYears(double value)
    {
        _revenueGrowth = value;
    }

    int EarningStability() const
    {
        return _earningStability;
    }

    void SetEarningStability(int value)
    {
        _earningStability = value;
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

    int DividendStability() const
    {
        return _dividendStability;
    }

    void SetDividendStability(int value)
    {
        _dividendStability = value;
    }

    double PayoutRatio() const
    {
        return _payoutRatio;
    }

    void SetPayoutRatio(double value)
    {
        _payoutRatio = value;
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

     double _percentage{0.0};
     int _revenueStability{0};
     double _revenueGrowth{0.0};
     int _earningStability{0};
     double _earningGrowth{0.0};
     int _dividendStability{0};
     double _dividendGrowth{0.0};
     double _payoutRatio{0.0};
};

#endif // STOCKENTITY_H
