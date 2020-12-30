#include "evaluator.h"
#include <algorithm>
#include <math.h>       /* sqrt */

static const unsigned numYears = 5;

int GetNumYearsDividendReduced(const StockEntity& stock);
int getPointsOfPayoutRatio(const StockEntity& stock);

class Evaluator
{
public:

    void CalculateAndEvaluate(std::vector<StockEntity>& stocks)
    {
        for (auto& stock : stocks)
        {
            // Order Data before doing anything
            std::vector<StockEntity::YearDataSet> data = stock.GetYearData();
            std::sort(data.begin(), data.end());
            stock.SetYearData(data);

            if (!Calculate(stock))
            {
                stock.AddRemark("Keine Auswertung");
                continue;
            }

            EvaluateStock(stock);
            doRating(stock);
        }
    }

private:

    double _minHold = 10;
    double _minBuy = 14;

	void doRating(StockEntity& stock)
	{
        if (stock.Percentage() >= _minBuy)
		{
            stock.SetRating(StockEntity::Rate::A);
            return;
		}
        else if (stock.Percentage() >= _minHold)
		{
            stock.SetRating(StockEntity::Rate::B);
            return;
		}
		stock.SetRating(StockEntity::Rate::C);
    }

    int GetScoreOfGrowth(double growth)
    {
        int score = 0;

        if(growth > 1.0)
            score++;
        if(growth > 2.0)
            score++;
        if(growth > 3.0)
            score++;
        if(growth > 4.0)
            score++;
        if(growth > 5.0)
            score++;

        return score;
    }

    void EvaluateStock(StockEntity& stock)
    {
        int score = 0;
        double growth = stock.RevenueGrowthFiveYears();
        if (growth <= 1.5)
            stock.AddRemark("Umsatzwachstum gering");
        score += GetScoreOfGrowth(growth);

        growth = stock.EarningGrowthFiveYears();
        if (growth <= 1.5)
            stock.AddRemark("Gewinnwachstum gering");
        score += GetScoreOfGrowth(growth);

        growth = stock.DividendGrowthFiveYears();
        if (growth <= 1.5)
            stock.AddRemark("Dividendenwachstum gering");
        score += GetScoreOfGrowth(growth);

        if (getPointsOfPayoutRatio(stock) <= 3)
            stock.AddRemark("Auschüttungsquote nicht gut");
        score += getPointsOfPayoutRatio(stock);

        stock.SetPercentag(score);
    }

    // --- functions for calculating ---------------------------------------

    bool Calculate(StockEntity& stock)
	{        
        if (stock.GetYearData().size() < numYears + 1)
        {
            return false;
        }

        if (!SetGrowths(stock)) return false;
		if (!SetPayoutRatio(stock)) return false;

        SetDividendRegression(stock);
        SetEarningRegression(stock);
        SetRevenueRegression(stock);

		return true;
	}

    bool SetPayoutRatio(StockEntity& stock)
    {
        // calculates the "cumulated" payout ratio
        if (stock.GetYearData().size() < 5) return false;

        double sumEarnings = stock.GetYearData()[stock.GetYearData().size() - 5].Earnings
                + stock.GetYearData()[stock.GetYearData().size() - 4].Earnings
                + stock.GetYearData()[stock.GetYearData().size() - 3].Earnings
                + stock.GetYearData()[stock.GetYearData().size() - 2].Earnings
                + stock.GetYearData()[stock.GetYearData().size() - 1].Earnings;

        double sumDividends = stock.GetYearData()[stock.GetYearData().size() - 5].Dividend
                + stock.GetYearData()[stock.GetYearData().size() - 4].Dividend
                + stock.GetYearData()[stock.GetYearData().size() - 3].Dividend
                + stock.GetYearData()[stock.GetYearData().size() - 2].Dividend
                + stock.GetYearData()[stock.GetYearData().size() - 1].Dividend;

        if (sumEarnings > 1.0e-6)
        {
            stock.SetPayoutRatio(100.0 * sumDividends / sumEarnings);
        }

        return true;
    }

    bool SetGrowths(StockEntity& stock)
    {
        if (stock.GetYearData().size() < 6) return false;

        // calculate three years earnings growth
        auto x0 = stock.GetYearData()[stock.GetYearData().size() - 6].Earnings;
        auto x1 = stock.GetYearData()[stock.GetYearData().size() - 1].Earnings;
        stock.SetEarningGrowthThreeYears(CompoundAnnualGrowthRate(x1, x0, 5));

        // calculate three years dividend growth
        x0 = stock.GetYearData()[stock.GetYearData().size() - 6].Dividend;
        x1 = stock.GetYearData()[stock.GetYearData().size() - 1].Dividend;
        stock.SetDividendGrowthThreeYears(CompoundAnnualGrowthRate(x1, x0, 5));

        // calculate three years revenue growth
        x0 = stock.GetYearData()[stock.GetYearData().size() - 6].Revenue;
        x1 = stock.GetYearData()[stock.GetYearData().size() - 1].Revenue;
        stock.SetRevenueGrowthThreeYears(CompoundAnnualGrowthRate(x1, x0, 5));

        return true;
    }

    bool SetDividendRegression(StockEntity& stock)
    {
        auto lambda = [] (const StockEntity::YearDataSet& entitiy) { return entitiy.Dividend; };
        double n = 0.0;
        double m = 0.0;
        GetRegressionParameters(stock, lambda, n, m);

        StockEntity::RegressionParames dividendRegParams;
        dividendRegParams.m = m;
        dividendRegParams.n = n;
        dividendRegParams.startYear = stock.GetYearData().back().Year - numYears;
        stock.SetDividendRegression(dividendRegParams);

        return true;
    }

    bool SetEarningRegression(StockEntity& stock)
    {
        auto lambda = [] (const StockEntity::YearDataSet& entitiy) { return entitiy.Earnings; };
        double n = 0.0;
        double m = 0.0;
        GetRegressionParameters(stock, lambda, n, m);

        StockEntity::RegressionParames earningRegParams;
        earningRegParams.m = m;
        earningRegParams.n = n;
        earningRegParams.startYear = stock.GetYearData().back().Year - numYears;
        stock.SetEarningRegression(earningRegParams);

        return true;
    }

    bool SetRevenueRegression(StockEntity& stock)
    {
        auto lambda = [] (const StockEntity::YearDataSet& entitiy) { return entitiy.Revenue; };
        double n = 0.0;
        double m = 0.0;
        GetRegressionParameters(stock, lambda, n, m);

        StockEntity::RegressionParames revenueRegParams;
        revenueRegParams.m = m;
        revenueRegParams.n = n;
        revenueRegParams.startYear = stock.GetYearData().back().Year - numYears;
        stock.SetRevenueRegression(revenueRegParams);

        return true;
    }

    template<typename Functor>
    bool GetCorrelationFactor(StockEntity& stock, Functor functor, int& outValue)
    {
        double growthThreshold = 1.0;

        auto yearDataSet = stock.GetYearData();
        unsigned startYear = yearDataSet.size() - (numYears + 1);

        int counter = 0;
        for (auto i = startYear; i < yearDataSet.size() - 1; i++) {
            auto growth = std::abs(functor(yearDataSet[i])) > 0.0001 ?
                                       (functor(yearDataSet[i + 1]) / functor(yearDataSet[i])) - 1 : 0.0;
            growth *= 100.0;
            counter += growth > growthThreshold ? 1 : 0;
        }

        outValue = counter;
        return true;
    }

    template<typename Functor>
    void GetRegressionParameters(const StockEntity& stock,
                                 Functor functor,
                                 double& n,
                                 double& m)
    {
        const std::vector<StockEntity::YearDataSet>& yearDataSet = stock.GetYearData();

        auto lambda = [] (const StockEntity::YearDataSet& entitiy) { return entitiy.Year; };

        double yearAverage = calcAverageOverNumYears(yearDataSet, lambda);
        double yAverage = calcAverageOverNumYears(yearDataSet, functor);

        m = calcParameterBOverNumYears(yearDataSet, yearAverage, yAverage, functor);
        n = yAverage - m*yearAverage;
    }

    // compound annual growth rate : CAGR = (EB / BB)^(1 / N) - 1
    // EB = Ending Balance
    // BB = Beginning Balance
    // N = Number of years
    double CompoundAnnualGrowthRate(double eb, double bb, int n)
    {
        if (std::abs(bb) < 1.0e-6 || n < 1)
        {
            return 0.0;
        }

        double quotient = eb / bb;
        if (quotient < 0.0)
        {
            quotient = (-1.0 * quotient);
            return -1.0 * pow(quotient, 1.0 / n) - 1.0;
        }

        double cagr = pow(quotient, 1.0 / n) - 1.0;
        return cagr * 100.0;
    }
};

int GetNumYearsDividendReduced(const StockEntity& stock)
{
    int yearCounter = 0;
    for (size_t i = stock.GetYearData().size() - numYears; i < stock.GetYearData().size(); ++i)
    {
        if (stock.GetYearData()[i].Dividend < stock.GetYearData()[i - 1].Dividend)
        {
            yearCounter++;
        }
    }
    return yearCounter;
}

int getPointsOfPayoutRatio(const StockEntity& stock)
{
    double payout = stock.PayoutRatio();
    int point = 0;

    if(payout >= 30.0 && payout <= 70.0)
        point++;
    if(payout >= 20.0 && payout <= 80.0)
        point++;
    if(payout >= 15.0 && payout <= 85.0)
        point++;
    if(payout >= 10.0 && payout <= 90.0)
        point++;
    if(payout >= 5.0 && payout <= 95.0)
        point++;

    return point;
}

template<typename Functor>
double calcAverageOverNumYears(const std::vector<StockEntity::YearDataSet>& data, Functor functor)
{
    double average = 0.0;
    for(unsigned i = data.size() - (numYears + 1); i < data.size(); ++i)
    {
        average += functor(data[i]);
    }
    average /= (numYears + 1);
    return average;
}

template<typename Functor>
double calcParameterBOverNumYears(const std::vector<StockEntity::YearDataSet>& data,
                                  double xRoof,
                                  double yRoof,
                                  Functor functor)
{
    double counter = 0.0;
    double denominator = 0.0;
    for(unsigned i = data.size() - (numYears + 1); i < data.size(); ++i)
    {
        counter += (data[i].Year - xRoof) * (functor(data[i]) - yRoof);
        denominator += (data[i].Year - xRoof) * (data[i].Year - xRoof);
    }
    return counter / denominator;
}

void CalculateAndEvaluate(std::vector<StockEntity>& stocks)
{
	Evaluator evaluator;
	evaluator.CalculateAndEvaluate(stocks);
}
