#include "Client.h"
#include <iostream>


void Client::InsertOrder(const Order& order) const
{
	m_engine->InsertOrder(id, order);
}

void Client::Notify(Ack& ack)
{
	std::cout << "Client " << id << " response from server: " << ack << std::endl;
}

void Client::Notify(OrderUpdate& orderUpdate)
{
	std::cout << "Client " << id << " received order update: " << orderUpdate << std::endl;
}
