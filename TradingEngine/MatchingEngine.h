#pragma once
#include <cstdint>
#include <memory>
#include <map>
#include <deque>
#include <mutex>
#include "Order.h"
#include "Ack.h"
#include "OrderBook.h"


enum class TransactionType { Insert, Amend, Cancel };

struct Transaction
{
	OrderId orderId;
	Order order;
	TransactionType type;
};


class MatchingEngine
{
public:
	MatchingEngine() {};

	void Start();

	void Stop();

	void InsertOrder(const Order& order);

	void AmendOrder(const OrderId& orderId, const Order& order);

	void CancelOrder(const OrderId& orderId);

	inline void ReceiveMarketData(const OrderBook& orderBook) { m_orderBook = std::make_shared<OrderBook>(orderBook); };

private:
	void AddTransactionToProcessingQueue(const Transaction& transaction);

	void ProcessQueue();

private:
	std::shared_ptr<OrderBook> m_orderBook{}; /*Current order book*/

	std::shared_ptr<std::deque<Transaction>> m_transactionsQueue{};
	mutable std::mutex m_mu;
	std::condition_variable m_cv;
	std::shared_ptr<std::thread> m_processingThread = 0;

	bool m_isProcessing = false;
};

