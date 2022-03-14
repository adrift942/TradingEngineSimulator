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
	
	/*Insert an order in the order book, The order is executed if marketable, the remaining unfilled amount is inserted as a pending order.*/
	Ack InsertOrder(const ClientId& clientId, const Order& order);

	/*Cancel an order given its ID and create a new order with the same ID.*/
	Ack AmendOrder(const ClientId& clientId, const OrderId& orderId, const Order& order);

	/*Removes an order from the order book given its ID.*/
	Ack CancelOrder(const ClientId& clientId, const OrderId& orderId);

	/*Receive a stream of orders that simulate a 5-level orderbook snapshot.*/
	void ReceiveMarketDataStream(const std::vector<Order>& orders);

	/*Subscribe client ID in order to notify it when an order is executed, modified or canceled.*/
	inline void SubscribeClient(const ClientId& clientId, const std::shared_ptr<IObserver> observer) { m_clientMap.emplace(clientId, observer); }

	void Stop();

	inline void SetIsTest(const bool isTest) { m_orderBook.SetIsTest(isTest); };

private:
	void AddTransactionToProcessingQueue(const Transaction& transaction);

	void ProcessTransactionsQueue();

	void StreamMarketData();

	void NotifyOrderUpdate(std::shared_ptr<OrderUpdate> orderUpdate);

	void NotifyTrade(std::shared_ptr<Trade> trade);

private:
	OrderBook m_orderBook{};

	std::shared_ptr<std::deque<Transaction>> m_transactionsQueue = 0; /*Queue of transactions to be processed*/
	mutable std::mutex m_mu;		/*Synchonize access to the transactions queue*/
	std::condition_variable m_cv;
	bool m_isProcessing = false;

	std::shared_ptr<std::thread> m_processingThread = 0;	/*Thread to process the transactions*/
	std::shared_ptr<std::thread> m_streamDataThread = 0;	/*Thread to generate test data*/

	std::map<ClientId, std::shared_ptr<IObserver>> m_clientMap{}; /*Map to notify the clients based on their ID*/

	uint64_t m_txPerSecondCounter = 0; /*Counter of the number of transactions per second*/
};
