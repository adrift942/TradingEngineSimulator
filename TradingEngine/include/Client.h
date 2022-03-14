#pragma once
#include "MatchingEngine.h"
#include "OrderUpdate.h"
#include "IObserver.h"
#include "Ack.h"

static ClientId clientIdCount = 100;

class Client : public IObserver
{
public:
	Client(const std::shared_ptr<MatchingEngine> engine);

	/*Insert order*/
	Ack InsertOrder(const Order& order) const;

	/*Modify an order previously inserted*/
	Ack AmendOrder(const OrderId& orderId, const Order& order) const;

	/*Cancel an order*/
	Ack CancelOrder(const OrderId& orderId) const;

	/*Get notification when an order is inserted, executed, modified or canceled*/
	void Notify(const OrderUpdate& orderUpdate) override;

	/*Get notification when a trade is executed*/
	void Notify(const Trade& trade) override;

public:
	ClientId id;

private:	
	std::shared_ptr<MatchingEngine> m_engine;
};

