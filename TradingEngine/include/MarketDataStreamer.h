#pragma once
#include "OrderBook.h"

class MarketDataStreamer
{
public:
	void GetData(OrderBook& orderBook) const;
};

