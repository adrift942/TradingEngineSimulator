#include "Client.h"
#include <iostream>


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
	std::cout << "Client ID " << id << " received order update: " << orderUpdate << std::endl;
}

void Client::Notify(const Trade& trade)
{
	std::cout << "Client ID " << id << " executed trade: " << trade << std::endl;
}
