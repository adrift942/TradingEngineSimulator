#include "MarketDataStreamer.h"
#include "Util.h"


void MarketDataStreamer::GetData(std::vector<Order>& orders) const
{
	orders.clear();
	orders.push_back(Order(true, Randomize(9.96f), Randomize(40000.f)));
	orders.push_back(Order(true, Randomize(9.97f), Randomize(2000.f)));
	orders.push_back(Order(true, Randomize(9.98f), Randomize(3000.f)));
	orders.push_back(Order(true, Randomize(9.99f), Randomize(8000.f)));
	orders.push_back(Order(true, Randomize(10.0f), Randomize(200.f)));
	orders.push_back(Order(false, Randomize(10.10f), Randomize(500.f)));
	orders.push_back(Order(false, Randomize(10.08f), Randomize(20000.f)));
	orders.push_back(Order(false, Randomize(10.06f), Randomize(6000.f)));
	orders.push_back(Order(false, Randomize(10.04f), Randomize(8000.f)));
	orders.push_back(Order(false, Randomize(10.02f), Randomize(10000.f)));
}
