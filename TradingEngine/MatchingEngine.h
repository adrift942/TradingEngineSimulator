#pragma once
#include "Order.h"
#include "Ack.h"
#include "OrderBook.h"
#include "IObserver.h"
#include <cstdint>
#include <memory>
#include <map>
#include <deque>
#include <mutex>
#include <functional>


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
	MatchingEngine();

	void Stop();

	void InsertOrder(const ClientId& clientId, const Order& order);

	void AmendOrder(const ClientId& clientId, const OrderId& orderId, const Order& order);

	void CancelOrder(const ClientId& clientId, const OrderId& orderId);

	inline void ReceiveMarketData(const OrderBook& orderBook) { m_orderBook = orderBook; };

	inline void SubscribeClient(ClientId clientId, std::shared_ptr<IObserver> observer) { m_clientMap.emplace(clientId, observer); }

private:
	void AddTransactionToProcessingQueue(const Transaction& transaction);

	void ProcessingQueue();

private:
	OrderBook m_orderBook{};

	std::shared_ptr<std::deque<Transaction>> m_transactionsQueue = 0;
	mutable std::mutex m_mu;
	std::condition_variable m_cv;
	std::shared_ptr<std::thread> m_processingThread = 0;
	bool m_isProcessing = false;

	std::map<ClientId, std::shared_ptr<IObserver>> m_clientMap{};
};
