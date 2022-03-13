#pragma once
#include "Common.h"

struct Trade
{
	ClientId clientId;
	OrderUpdateType orderUpdateType;
	Order order;

	friend std::ostream& operator<<(std::ostream& os, const Trade& orderUpdate)
	{

		return os;
	}
};