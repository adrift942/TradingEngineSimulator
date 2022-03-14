#pragma once
#include "MatchingEngine.h"
#include "OrderUpdate.h"
#include "IObserver.h"

static ClientId clientIdCount = 100;

class Client : public IObserver
{
public:
	Client(const std::shared_ptr<MatchingEngine> engine);

	/*Insert order*/
	void InsertOrder(const Order& order) const;

	/*Modify an order previously inserted*/
	void AmendOrder(const OrderId& orderId, const Order& order) const;

	/*Cancel an order*/
	void CancelOrder(const OrderId& orderId) const;

	/*Receive response from server after requesting a transaction*/
	void Notify(const Ack& ack) override;

	/*Get notification when an order is inserted, executed, modified or canceled*/
	void Notify(const OrderUpdate& orderUpdate) override;

	/*Get notification when a trade is executed*/
	void Notify(const Trade& trade) override;

public:
	ClientId id;

private:	
	std::shared_ptr<MatchingEngine> m_engine;
};

