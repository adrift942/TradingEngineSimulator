#include "MatchingEngine.h"


// PUBLIC METHODS
void MatchingEngine::InsertOrder(const Order& i_order)
{
	// validation

	// if validation ok

	auto order = i_order;
	float tradeAmount;

	// try match order
	
}

void MatchingEngine::AmendOrder(const OrderId& orderId, const Order& order)
{
	// validation

	// if validation ok
	m_clientOrders[orderId] = order;
}

void MatchingEngine::CancelOrder(const OrderId& orderId)
{
	// validation

	// if validation ok
	m_clientOrders.erase(orderId);
}


// PRIVATE METHODS
