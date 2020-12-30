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
    double _minBuy = 16;

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

    void EvaluateStock(StockEntity& stock)
    {
        int score = stock.RevenueStability();
        if (score <= 3)
            stock.AddRemark("Umsatzstabilität gering");

        int scoreTmp = stock.EarningStability();
        if (scoreTmp <= 3)
            stock.AddRemark("Gewinnstabilität gering");
        score += scoreTmp;

        stock.SetDividendStability(stock.DividendStability() - GetNumYearsDividendReduced(stock));
        scoreTmp = stock.DividendStability();
        if (scoreTmp <= 3)
            stock.AddRemark("Dividendentabilität gering");
        score += scoreTmp;

        scoreTmp = getPointsOfPayoutRatio(stock);
        if (scoreTmp <= 3)
            stock.AddRemark("Auschüttungsquote nicht gut");
        score += scoreTmp;

        stock.SetPercentag(score);
    }

    // --- functions for calculating ---------------------------------------

    bool Calculate(StockEntity& stock)
	{        
        if (stock.GetYearData().size() < numYears)
        {
            return false;
        }
        if (!SetEarningStability(stock)) return false;
        if (!SetRevenueStability(stock)) return false;
        if (!SetDividendStability(stock)) return false;
        if (!SetGrowths(stock)) return false;
		if (!SetPayoutRatio(stock)) return false;

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
        stock.SetEarningGrowthThreeYears(CompoundAnnualGrowthRate(x1, x0, 3));

        // calculate three years dividend growth
        x0 = stock.GetYearData()[stock.GetYearData().size() - 6].Dividend;
        x1 = stock.GetYearData()[stock.GetYearData().size() - 1].Dividend;
        stock.SetDividendGrowthThreeYears(CompoundAnnualGrowthRate(x1, x0, 3));

        // calculate three years revenue growth
        x0 = stock.GetYearData()[stock.GetYearData().size() - 6].Revenue;
        x1 = stock.GetYearData()[stock.GetYearData().size() - 1].Revenue;
        stock.SetRevenueGrowthThreeYears(CompoundAnnualGrowthRate(x1, x0, 3));

        return true;
    }

    bool SetEarningStability(StockEntity& stock)
    {
        auto lambda = [] (const StockEntity::YearDataSet& entitiy) { return entitiy.Earnings; };
        int stability = 0;
        if(GetCorrelationFactor(stock, lambda, stability))
        {
            stock.SetEarningStability(stability);
            return true;
        }
        return false;
    }

    bool SetRevenueStability(StockEntity& stock)
    {
        auto lambda = [] (const StockEntity::YearDataSet& entitiy) { return entitiy.Revenue; };
        int stability = 0;
        if(GetCorrelationFactor(stock, lambda, stability))
        {
            stock.SetRevenueStability(stability);
            return true;
        }
        return false;
    }

    bool SetDividendStability(StockEntity& stock)
    {
        auto lambda = [] (const StockEntity::YearDataSet& entitiy) { return entitiy.Dividend; };
        int stability = 0;
        if(GetCorrelationFactor(stock, lambda, stability))
        {
            stock.SetDividendStability(stability);
            return true;
        }
        return false;
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

void CalculateAndEvaluate(std::vector<StockEntity>& stocks)
{
	Evaluator evaluator;
	evaluator.CalculateAndEvaluate(stocks);
}
