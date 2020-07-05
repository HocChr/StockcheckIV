#ifndef STOCKCHECK_H
#define STOCKCHECK_H

#include "idatabase.h"
#include "evaluator.h"
#include <memory>

class Stockcheck
{
	IDatabase* _databaseHandler;

public:
	
	Stockcheck(IDatabase* databaseHandler) : _databaseHandler(databaseHandler)
	{
	};

	std::vector<StockEntity> GetStocksList()
	{
		std::vector<StockEntity> stocks = _databaseHandler->GetStocks();

		CalculateAndEvaluate(stocks);

		return stocks;
	}
	
};


#endif // !STOCKCHECK_H
