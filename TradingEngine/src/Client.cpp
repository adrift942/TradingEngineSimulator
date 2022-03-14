#include "Client.h"
#include <iostream>


Client::Client(const std::shared_ptr<MatchingEngine> engine) : m_engine{ engine }
{
	id = clientIdCount++;
	engine->SubscribeClient(id, std::make_shared<Client>(*this));
}

void Client::InsertOrder(const Order& order) const
{
	m_engine->InsertOrder(id, order);
}

void Client::Notify(const Ack& ack)
{
	std::cout << "Client ID " << id << " response from server: " << ack << std::endl;
}

void Client::Notify(const OrderUpdate& orderUpdate)
{
	std::cout << "Client ID " << id << " received order update notification: " << orderUpdate << std::endl;
}

void Client::Notify(const Trade& trade)
{
	std::cout << "Client ID " << id << " received trade notification: " << trade << std::endl;
}
