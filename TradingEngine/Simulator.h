#pragma once
#include "MatchingEngine.h"
#include "Client.h"
#include "MarketDataStreamer.h"

class Simulator
{
public:
	Simulator(const MatchingEngine& engine, const Client& client);

	void Start();

private:
	MatchingEngine m_engine;
	Client m_client;
	MarketDataStreamer m_marketDataStreamer;
};

