#include "Client.h"
#include <iostream>


Client::Client(const std::shared_ptr<MatchingEngine> engine) : m_engine{ engine }
{
	id = clientIdCount++;
	engine->SubscribeClient(id, std::make_shared<Client>(*this));
}

Ack Client::InsertOrder(const Order& order) const
{
	return m_engine->InsertOrder(id, order);
}

Ack Client::AmendOrder(const OrderId& orderId, const Order& order) const
{
	return m_engine->AmendOrder(id, orderId, order);
}

Ack Client::CancelOrder(const OrderId& orderId) const
{
	return m_engine->CancelOrder(id, orderId);
}

void Client::Notify(const OrderUpdate& orderUpdate)
{
	//std::cout << "Client ID " << id << " received order update notification: " << orderUpdate << std::endl;
}

void Client::Notify(const Trade& trade)
{
	//std::cout << "Client ID " << id << " received trade notification: " << trade << std::endl;
}
