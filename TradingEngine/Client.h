#pragma once
#include "MatchingEngine.h"
#include "OrderUpdate.h"
#include "IObserver.h"

static ClientId clientIdCount = 0;

class Client : public IObserver
{
public:
	Client(const std::shared_ptr<MatchingEngine> engine) : m_engine{engine}
	{
		id = clientIdCount++;
	};

	void InsertOrder(const Order& order) const;

	void Notify(Ack& ack) override;

	void Notify(OrderUpdate& orderUpdate) override;

public:
	ClientId id;

private:	
	std::shared_ptr<MatchingEngine> m_engine;
};

