#pragma once
#include "Order.h"
#include "Util.h"
#include "OrderUpdate.h"
#include <deque>
#include <forward_list>
#include <map>
#include <mutex>
#include <functional>


class OrderBook
{
public:
	float GetBestBidPrice() const;

	float GetBestAskPrice() const;

	float GetBestBidAmount() const;

	float GetBestAskAmount() const;

	bool OrderExists(const OrderId& orderId) const;

	void InsertOrder(const Order& i_order);
	
	bool AmendOrder(const OrderId& orderId, const Order& i_order);

	bool CancelOrder(const OrderId& orderId);

	void SetOrderUpdateCallback(std::function<void(OrderUpdate*)> callback);

	inline void SetTest(bool isTest) { m_isTest = isTest; };

	bool operator==(const OrderBook& other) const;

	bool operator!=(const OrderBook& other) const;

private:
	void ExecuteOrder(Order& order);

	void InsertPendingOrder(Order& order);

	bool CancelOrderFromQueue(const OrderId& orderId, std::deque<Order>& queue);

	void NotifyOrderUpdate(const OrderUpdate& orderUpdate);

public:
	std::forward_list<std::deque<Order>> m_bids;
	std::forward_list<std::deque<Order>> m_asks;

private:
	std::map<OrderId, Order> m_ordersMap; // auxiliary map for faster lookup of orders in the orderbook	
	std::function<void(OrderUpdate*)> m_callback;
	bool m_isTest = false; // avoid calling callbacks during unit tests
};
