#pragma once
#include "Order.h"
#include <vector>


class MarketDataStreamer
{
public:
	void GetData(std::vector<Order>& orders) const;
};

