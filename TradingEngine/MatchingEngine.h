#pragma once
#include <cstdint>
#include <memory>
#include <map>
#include <deque>
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

	inline void ReceiveMarketData(const OrderBook& orderBook) { m_orderBook = std::make_shared<OrderBook>(orderBook); };

private:
	std::shared_ptr<OrderBook> m_orderBook{}; /*Current order book*/

	std::shared_ptr<std::deque<Order>> m_ordersQueue{};
};

