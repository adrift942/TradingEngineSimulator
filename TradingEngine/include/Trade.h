#pragma once
#include "Common.h"

struct Trade
{
	ClientId clientId;
	bool isBuy;
	float price;
	float amount;

	friend std::ostream& operator<<(std::ostream& os, const Trade& trade)
	{
		std::string side = trade.isBuy ? "bought" : "sold";
		return os << side << " " << trade.amount << " at " << trade.price;
	}
};