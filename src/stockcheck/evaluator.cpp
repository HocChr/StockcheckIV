#include "evaluator.h"
#include <algorithm>
#include <math.h>       /* sqrt */


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

            if(isNoDividendStock(stock))
                EvaluateGrowthStock(stock);
            else
                EvaluateDividendStock(stock);

            doRating(stock);
        }
    }

private:

    double _minHold = 0.5;
    double _minBuy = 0.8;

	void doRating(StockEntity& stock)
	{
        if (stock.Percentage() >= _minBuy * 100.0)
		{
            stock.SetRating(StockEntity::Rate::A);
            return;
		}
        else if (stock.Percentage() >= _minHold * 100.0)
		{
            stock.SetRating(StockEntity::Rate::B);
            return;
		}
		stock.SetRating(StockEntity::Rate::C);
    }

    void EvaluateGrowthStock(StockEntity& stock)
    {
        double percentage = 0.0, percentageTmp = 0.0;

        percentageTmp = PercentageOfEarningCorrelation(stock, 0.5);
        if (percentageTmp < _minHold)
            stock.AddRemark("Gewinnkorrelation gering");
        percentage = percentageTmp;

        percentageTmp = PercentageOfEarningGrowth(stock, 10.0);
        if (percentageTmp < _minHold)
            stock.AddRemark("Gewinnwachstum gering");
        percentage += percentageTmp;

        if(percentage > _minHold)
            stock.SetStockType(StockEntity::StockType::GrowthStock);

        stock.SetPercentag(100.0 * percentage / 2.0);
    }

    void EvaluateDividendStock(StockEntity& stock)
    {
        double percentage = 0.0, percentageTmp = 0.0;

        percentageTmp = PercentageOfEarningCorrelation(stock);
        if (percentageTmp < _minHold)
            stock.AddRemark("Gewinnkorrelation gering");
        percentage = percentageTmp;

        percentageTmp = PercentageOfEarningGrowth(stock);
        if (percentageTmp < _minHold)
            stock.AddRemark("Gewinnwachstum gering");
        percentage += percentageTmp;

        percentageTmp = PercentageOfDividendGrowth(stock);
        if (percentageTmp < _minHold)
            stock.AddRemark("Dividendenwachstum gering");
        percentage += percentageTmp;

        percentageTmp = PercentageOfDividendYearsNotCutted(stock);
        if (percentageTmp < _minHold)
            stock.AddRemark("Dividende nicht stabil");
        percentage += percentageTmp;

        percentageTmp = PercentageOfPayoutRatio(stock);
        if (percentageTmp < _minHold)
            stock.AddRemark("Auschüttungsquote nicht gut");
        percentage += percentageTmp;

        if(percentage > _minHold)
            stock.SetStockType(StockEntity::StockType::DivididendStock);

        stock.SetPercentag(100.0 * percentage / 5.0);
    }

    double PercentageOfEarningCorrelation(const StockEntity& stock, double minimalCorrelation = 0.0)
    {
        double maximalCorrelation = 1.0;

        return CalcPercentage(minimalCorrelation, maximalCorrelation, stock.EarningCorrelation());
    }

    double PercentageOfEarningGrowth(const StockEntity& stock, double maximalGrowth = 5.0)
    {
        double minimalGrowth = 0.0;

        return CalcPercentage(minimalGrowth, maximalGrowth, stock.EarningGrowthThreeYears());
    }

    double PercentageOfDividendGrowth(const StockEntity& stock)
    {
        double minimalGrowth = 0.0;
        double maximalGrowth = 5.0;

        return CalcPercentage(minimalGrowth, maximalGrowth, stock.DividendGrowthThreeYears());
    }

    double PercentageOfDividendYearsNotCutted(const StockEntity& stock)
    {
        int minimalYears = 0;
        int maximalYears = 10;

        return CalcPercentage(minimalYears, maximalYears, stock.NumYearsDividendNotReduced());
    }

    double PercentageOfPayoutRatio(StockEntity& stock)
    {
        auto percentage0 = CalcPercentage(0.0, 40.0, stock.PayoutRatio());
        auto percentage1 = 1.0 - CalcPercentage(60.0, 100.0, stock.PayoutRatio());

        auto best = std::min(percentage0, percentage1);
        return best;
    }

	double CalcPercentage(double minExpected, double maxExpected, double actualValue)
	{
		if (actualValue <= minExpected)
		{
			return 0.0;
		}
		else if (actualValue >= maxExpected)
		{
			return 1.0;
		}
		else if ((maxExpected - minExpected) < 1.0e-6)
		{
			return 0.0;
		}
		else
		{
			double m = 1.0 / (maxExpected - minExpected);
			return m * actualValue - m * minExpected;
		}
	}

    // --- functions for calculating ---------------------------------------

    bool isNoDividendStock(const StockEntity& stock)
    {
        return stock.PayoutRatio() < 0.1 &&
            stock.NumYearsDividendNotReduced() == static_cast<int>(stock.GetYearData().size()) - 1;
    }

	bool Calculate(StockEntity& stock)
	{
		if (!SetEarningCorrelationFactor(stock)) return false;
		if (!SetEarningAndDividendGrowth(stock)) return false;
		if (!SetPayoutRatio(stock)) return false;

		SetNumYearsDividendNotReduced(stock);

		return true;
	}

    void SetNumYearsDividendNotReduced(StockEntity& stock)
    {
        int numYears = 0;
        for (size_t i = 1; i < stock.GetYearData().size(); i++)
        {
            if (stock.GetYearData()[i - 1].Dividend <= stock.GetYearData()[i].Dividend)
            {
                numYears++;
            }
        }
        stock.SetNumYearsDividendNotReduced(numYears);
    }
    
    bool SetPayoutRatio(StockEntity& stock)
    {
        // calculates the "cumulated" payout ratio
        if (stock.GetYearData().size() < 3) return false;

        double sumEarnings = stock.GetYearData()[stock.GetYearData().size() - 3].Earnings
            + stock.GetYearData()[stock.GetYearData().size() - 2].Earnings
            + stock.GetYearData()[stock.GetYearData().size() - 1].Earnings;

        double sumDividends = stock.GetYearData()[stock.GetYearData().size() - 3].Dividend
            + stock.GetYearData()[stock.GetYearData().size() - 2].Dividend
            + stock.GetYearData()[stock.GetYearData().size() - 1].Dividend;

        if (sumEarnings > 1.0e-6)
        {
            stock.SetPayoutRatio(100.0 * sumDividends / sumEarnings);
        }

        return true;
    }

    bool SetEarningAndDividendGrowth(StockEntity& stock)
    {
        if (stock.GetYearData().size() < 6) return false;

        // calculate three years earnings growth
        auto x0 = stock.GetYearData()[stock.GetYearData().size() - 4].Earnings;
        auto x1 = stock.GetYearData()[stock.GetYearData().size() - 1].Earnings;
        stock.SetEarningGrowthThreeYears(CompoundAnnualGrowthRate(x1, x0, 3));

        // calculate three years dividend growth
        x0 = stock.GetYearData()[stock.GetYearData().size() - 4].Dividend;
        x1 = stock.GetYearData()[stock.GetYearData().size() - 1].Dividend;
        stock.SetDividendGrowthThreeYears(CompoundAnnualGrowthRate(x1, x0, 3));

        return true;
    }

    bool SetEarningCorrelationFactor(StockEntity& stock)
    {
        double xDach = 0.0;
        double tDach = 0.0;
        double pXTdach = 0.0;
        double dXDachsqrt = 0.0;
        double dTDachsqrt = 0.0;
        double r = -1.0;

        // ---- Prepare and check ---------------------
        auto _list = stock.GetYearData();
        if (_list.size() == 0)
        {
            stock.AddRemark("Korrelationsberechnung: Abbruch. Zu wenig Daten");
            return false;
        }
        // --- do calculation -------------------------
        for(const auto& item : _list)
        {
            xDach = xDach + item.Earnings;
            tDach = tDach + item.Year;
        }
        xDach = xDach / _list.size();
        tDach = tDach / _list.size();
        // --------------------------------------------
        for(const auto& item : _list)
        {
            auto tmp1 = item.Year - tDach;
            auto tmp2 = item.Earnings - xDach;
            pXTdach = pXTdach + tmp1 * tmp2;
            dXDachsqrt = dXDachsqrt + tmp2 * tmp2;
            dTDachsqrt = dTDachsqrt + tmp1 * tmp1;
        }
        // --------------------------------------------
        if (sqrt(dXDachsqrt) * sqrt(dTDachsqrt) > 0.1)
        {
            r = pXTdach / (sqrt(dXDachsqrt) * sqrt(dTDachsqrt));
            stock.SetEarningCorrelation(r);
        }
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

void CalculateAndEvaluate(std::vector<StockEntity>& stocks)
{
	Evaluator evaluator;
	evaluator.CalculateAndEvaluate(stocks);
}
