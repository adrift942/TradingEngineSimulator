#include "MarketDataStreamer.h"

void MarketDataStreamer::GetData(std::vector<Order>& orders) const
{
	orders.clear();
	orders.push_back(Order(true, 9.96f, 40000.f));
	orders.push_back(Order(true, 9.97f, 2000.f));
	orders.push_back(Order(true, 9.98f, 3000.f));
	orders.push_back(Order(true, 9.99f, 8000.f));
	orders.push_back(Order(true, 10.f, 200.f));
	orders.push_back(Order(false, 10.10f, 500.f));
	orders.push_back(Order(false, 10.08f, 20000.f));
	orders.push_back(Order(false, 10.06f, 6000.f));
	orders.push_back(Order(false, 10.04f, 8000.f));
	orders.push_back(Order(false, 10.02f, 10000.f));
}
