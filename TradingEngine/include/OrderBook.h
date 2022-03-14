#pragma once
#include "Order.h"
#include "Util.h"
#include "OrderUpdate.h"
#include "Trade.h"
#include <deque>
#include <forward_list>
#include <map>
#include <mutex>
#include <functional>


class OrderBook
{
public:
	OrderBook() {};

	OrderBook(std::forward_list<std::deque<Order>>& bids, std::forward_list<std::deque<Order>>& asks);

	/*Get price of the highest bid in the book*/
	float GetBestBidPrice() const;

	/*Get price of the lowest ask in the book*/
	float GetBestAskPrice() const;

	/*Check if the order ID exists in the book*/
	bool OrderExists(const OrderId& orderId) const;

	/*Insert an order in the order book, The order is executed if marketable, the remaining unfilled amount is inserted as a pending order.*/
	void InsertOrder(const Order& i_order);
	
	/*Cancel an order given its ID and create a new order with the same ID.*/
	bool AmendOrder(const OrderId& orderId, const Order& i_order);

	/*Removes an order from the order book given its ID.*/
	bool CancelOrder(const OrderId& orderId);

	/*To call Notify of MatchingEngine when an order is updated*/
	void SetCallback(std::function<void(std::shared_ptr<OrderUpdate>)> callback);

	/*To call Notify of MatchingEngine when a trade is executed*/
	void SetCallback(std::function<void(std::shared_ptr<Trade>)> callback);

	/*Avoid calling callbacks when UnitTesting*/
	inline void SetIsTest(const bool isTest) { m_isTest = isTest; };

	bool operator==(const OrderBook& other) const;

	bool operator!=(const OrderBook& other) const;

private:
	void ExecuteOrder(Order& order);

	void InsertPendingOrder(Order& order);

	void Notify(const OrderUpdate& orderUpdate);

	void Notify(const Trade& trade);

private:
	std::forward_list<std::deque<Order>> m_bids; /*List ordered by descending price of queues of bid orders*/
	std::forward_list<std::deque<Order>> m_asks; /*List ordered by ascending price of queues of ask orders*/

	std::map<OrderId, Order> m_ordersMap; /*Auxiliary map for faster lookup of orders in the orderbook*/

	std::function<void(std::shared_ptr<OrderUpdate>)> m_orderUpdateCallback;
	std::function<void(std::shared_ptr<Trade>)> m_tradeCallback;

	bool m_isTest = false;
};
