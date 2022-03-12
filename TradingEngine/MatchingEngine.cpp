#include "MatchingEngine.h"


// PUBLIC METHODS
void MatchingEngine::InsertOrder(const Order& i_order)
{
	// validation

	// if validation ok
		
	m_ordersQueue->push_back(i_order);	
}

void MatchingEngine::AmendOrder(const OrderId& orderId, const Order& order)
{
	// validation

	// if validation ok
	m_orderBook->AmendOrder(orderId, order);
}

void MatchingEngine::CancelOrder(const OrderId& orderId)
{
	// validation

	// if validation ok
	m_orderBook->CancelOrder(orderId);
}


// PRIVATE METHODS
