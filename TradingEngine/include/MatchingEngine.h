#pragma once
#include "Order.h"
#include "Ack.h"
#include "OrderBook.h"
#include "IObserver.h"
#include "Trade.h"
#include <cstdint>
#include <memory>
#include <map>
#include <deque>
#include <mutex>
#include <functional>
#include <vector>


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

	void ReceiveMarketDataStream(const std::vector<Order>& orders);

	inline void SubscribeClient(const ClientId& clientId, const std::shared_ptr<IObserver> observer) { m_clientMap.emplace(clientId, observer); }

	inline void SetIsTest(const bool isTest) { m_orderBook.SetIsTest(isTest); };

private:
	void AddTransactionToProcessingQueue(const Transaction& transaction);

	void ProcessTransactionsQueue();

	void StreamMarketData();

	void NotifyAck(const ClientId& clientId, const Ack& ack);

	void NotifyOrderUpdate(std::shared_ptr<OrderUpdate> orderUpdate);

	void NotifyTrade(std::shared_ptr<Trade> trade);

private:
	OrderBook m_orderBook{};

	std::shared_ptr<std::deque<Transaction>> m_transactionsQueue = 0;
	mutable std::mutex m_mu;
	std::condition_variable m_cv;
	bool m_isProcessing = false;

	std::shared_ptr<std::thread> m_processingThread = 0;
	std::shared_ptr<std::thread> m_streamDataThread = 0;

	std::map<ClientId, std::shared_ptr<IObserver>> m_clientMap{};

	uint64_t m_txPerSecondCounter = 0;
};
