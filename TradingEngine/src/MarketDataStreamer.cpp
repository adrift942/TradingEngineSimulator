#include "MarketDataStreamer.h"

float getRandomNumber()
{
	float LO = 0.7f;
	float HI = 1.5f;
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

float round2(float n)
{
	return round(n * 100) / 100;
}

float randomize(float n)
{
	return round2(n * getRandomNumber());
}

void MarketDataStreamer::GetData(std::vector<Order>& orders) const
{
	orders.clear();
	orders.push_back(Order(true, randomize(9.96f), randomize(40000.f)));
	orders.push_back(Order(true, randomize(9.97f), randomize(2000.f)));
	orders.push_back(Order(true, randomize(9.98f), randomize(3000.f)));
	orders.push_back(Order(true, randomize(9.99f), randomize(8000.f)));
	orders.push_back(Order(true, randomize(10.0f), randomize(200.f)));
	orders.push_back(Order(false, randomize(10.10f), randomize(500.f)));
	orders.push_back(Order(false, randomize(10.08f), randomize(20000.f)));
	orders.push_back(Order(false, randomize(10.06f), randomize(6000.f)));
	orders.push_back(Order(false, randomize(10.04f), randomize(8000.f)));
	orders.push_back(Order(false, randomize(10.02f), randomize(10000.f)));
	//orders.push_back(Order(true, 9.96f, 40000.f));
	//orders.push_back(Order(true, 9.97f, 2000.f));
	//orders.push_back(Order(true, 9.98f, 3000.f));
	//orders.push_back(Order(true, 9.99f, 8000.f));
	//orders.push_back(Order(true, 10.0f, 200.f));
	//orders.push_back(Order(false, 10.10f, 500.f));
	//orders.push_back(Order(false, 10.08f, 20000.f));
	//orders.push_back(Order(false, 10.06f, 6000.f));
	//orders.push_back(Order(false, 10.04f, 8000.f));
	//orders.push_back(Order(false, 10.02f, 10000.f));
}
