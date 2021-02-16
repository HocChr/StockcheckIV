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

    struct RegressionParames{
        double m;
        double n;
        int startYear;
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

    const RegressionParames& RevenueRegression() const
    {
        return _revenueRegression;
    }

    void SetRevenueRegression(RegressionParames value)
    {
        _revenueRegression = value;
    }

    double RevenueGrowthFiveYears() const
    {
        return _revenueGrowthFiveYears;
    }

    void SetRevenueGrowthFiveYears(double value)
    {
        _revenueGrowthFiveYears = value;
    }

    double RevenueGrowthOneYear() const
    {
        return _revenueGrowthOneYear;
    }

    void SetRevenueGrowthOneYear(double value)
    {
        _revenueGrowthOneYear = value;
    }

    const RegressionParames& EarningRegression() const
    {
        return _earningRegression;
    }

    void SetEarningRegression(RegressionParames value)
    {
        _earningRegression = value;
    }

    double EarningGrowthFiveYears() const
    {
        return _earningGrowthFiveYears;
    }

    void SetEarningGrowthFiveYears(double value)
    {
        _earningGrowthFiveYears = value;
    }

    double EarningGrowthOneYear() const
    {
        return _earningGrowthOneYear;
    }

    void SetEarningGrowthOneYear(double value)
    {
        _earningGrowthOneYear = value;
    }

    double DividendGrowthFiveYears() const
    {
        return _dividendGrowthFiveYears;
    }

    void SetDividendGrowthFiveYears(double value)
    {
        _dividendGrowthFiveYears = value;
    }

    double DividendGrowthOneYear() const
    {
        return _dividendGrowthOneYear;
    }

    void SetDividendGrowthOneYear(double value)
    {
        _dividendGrowthOneYear = value;
    }

    const RegressionParames& DividendRegresssion() const
    {
        return _dividendRegression;
    }

    void SetDividendRegression(RegressionParames value)
    {
        _dividendRegression = value;
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
     RegressionParames _revenueRegression{0.0, 0.0};
     double _revenueGrowthFiveYears{0.0};
     double _revenueGrowthOneYear{0.0};
     RegressionParames _earningRegression{0.0, 0.0};
     double _earningGrowthFiveYears{0.0};
     double _earningGrowthOneYear{0.0};
     RegressionParames _dividendRegression{0.0, 0.0};
     double _dividendGrowthFiveYears{0.0};
     double _dividendGrowthOneYear{0.0};
     double _payoutRatio{0.0};
};

#endif // STOCKENTITY_H
