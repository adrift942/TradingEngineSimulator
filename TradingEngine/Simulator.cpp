#include "Simulator.h"

Simulator::Simulator(const MatchingEngine& engine, const Client& client)
{
	m_engine = engine;
	m_client = client;
	m_marketDataStreamer = MarketDataStreamer();
}

void Simulator::Start()
{
	for (uint64_t i = 0; i < 1000; i++)
	{
		auto data = m_marketDataStreamer.GetData();
		m_engine.ReceiveMarketData(data);

		// client random actions
		auto order = Order(true, 10.05, 19000);
		m_client.InsertOrder(order);
	}
}
