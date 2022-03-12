#pragma once
#include <cstdint>
#include <memory>
#include <map>
#include "Order.h"
#include "Ack.h"
#include "OrderBook.h"


class MatchingEngine
{
public:
	MatchingEngine() {};

	void InsertOrder(const Order& order);

	void AmendOrder(const OrderId& orderId, const Order& order);

	void CancelOrder(const OrderId& orderId);

	inline void ReceiveMarketData(const OrderBook& orderBook) { m_orderBook = orderBook; };

private:
	OrderBook m_orderBook{}; /*Current order book*/

	std::map<OrderId, Order> m_clientOrders{}; /*Maps pending order ids to orders*/
};

