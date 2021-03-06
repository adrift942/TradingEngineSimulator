#include "MatchingEngine.h"
#include "MarketDataStreamer.h"
#include "OrderUpdate.h"
#include <thread>
#include <sstream>
#include <chrono>

using namespace std::chrono_literals;

static OrderId orderIdCount = 1000;


MatchingEngine::MatchingEngine()
{
	auto orderUpdateCallback = std::bind(&MatchingEngine::NotifyOrderUpdate, this, std::placeholders::_1);
	m_orderBook.SetCallback(orderUpdateCallback);
	auto tradeCallback = std::bind(&MatchingEngine::NotifyTrade, this, std::placeholders::_1);
	m_orderBook.SetCallback(tradeCallback);

	// start processing thread
	m_isProcessing = true;
	m_transactionsQueue.reset(new std::deque<Transaction>());
	m_processingThread.reset(new std::thread(&MatchingEngine::ProcessTransactionsQueue, this));
	
	// stream market data thread
	m_streamDataThread.reset(new std::thread(&MatchingEngine::StreamMarketData, this));
}

// PUBLIC METHODS
Ack MatchingEngine::InsertOrder(const ClientId& clientId, const Order& i_order)
{
	// assuming the client has enough balance to place the order
	auto order = i_order;
	order.id = orderIdCount++;
	order.clientId = clientId;
	std::ostringstream ss;
	ss << order;

	// link this order id to client id
	m_ordersOwnershipMap[order.id] = clientId;

	AddTransactionToProcessingQueue(Transaction{ order.id, order, TransactionType::Insert });

	return Ack{ true, "Insert " + ss.str(), order.id };
}

Ack MatchingEngine::AmendOrder(const ClientId& clientId, const OrderId& orderId, const Order& i_order)
{
	Ack ack;
	
	// validation
	if (m_ordersOwnershipMap.count(orderId) > 0 && m_ordersOwnershipMap[orderId] == clientId)
	{		
		std::ostringstream ss;
		auto order = i_order;
		order.id = orderId;
		ss << order;
		ack = Ack{ true, "Amend " + ss.str(), orderId };
		AddTransactionToProcessingQueue({ orderId, order, TransactionType::Amend });
	}
	else
	{
		ack = Ack{ false, "Could not amend order " + std::to_string(orderId), orderId };
	}
	
	return ack;
}

Ack MatchingEngine::CancelOrder(const ClientId& clientId, const OrderId& orderId)
{
	Ack ack;

	// validation
	if (m_ordersOwnershipMap.count(orderId) > 0 && m_ordersOwnershipMap[orderId] == clientId)
	{
		m_ordersOwnershipMap.erase(orderId);
		ack = Ack{ true, "Cancel order with ID " + std::to_string(orderId), orderId };
		AddTransactionToProcessingQueue({ orderId, Order(), TransactionType::Cancel });
	}
	else
	{
		ack = Ack{ false, "Could not cancel order ID " + std::to_string(orderId), orderId };
	}

	return ack;
}

void MatchingEngine::ReceiveMarketDataStream(const std::vector<Order>& orders)
{
	for (auto i = 0; i < orders.size(); i++)
	{
		Transaction tx = { 0, orders[i], TransactionType::Insert };
		AddTransactionToProcessingQueue(tx);
	}
}

void MatchingEngine::Stop()
{
	m_isProcessing = false;
	m_processingThread->join();
}

// PRIVATE METHODS
void MatchingEngine::AddTransactionToProcessingQueue(const Transaction& transaction)
{
	std::unique_lock<std::mutex> locker(m_mu);
	m_transactionsQueue->push_back(transaction);
	locker.unlock();
	m_cv.notify_one();
}

void MatchingEngine::ProcessTransactionsQueue()
{
	auto start = std::chrono::high_resolution_clock::now();
	while (m_isProcessing)
	{
		std::unique_lock<std::mutex> lock(m_mu);
		if (m_transactionsQueue->empty())
			m_cv.wait_for(lock, std::chrono::milliseconds(100)); // to avoid cpu overclocking if the queue is empty
		
		if (!m_transactionsQueue->empty())
		{
			const auto& transaction = m_transactionsQueue->front();
			m_transactionsQueue->pop_front();
			m_trxCounter++;

			switch (transaction.type)
			{
			case TransactionType::Insert:
				m_orderBook.InsertOrder(transaction.order);
				break;
			case TransactionType::Amend:
				m_orderBook.AmendOrder(transaction.orderId, transaction.order);
				break;
			case TransactionType::Cancel:
				m_orderBook.CancelOrder(transaction.orderId);
				break;
			}
		}
		
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
		if (duration.count() >= 1000)
		{
			std::cout << "Processed " << m_trxCounter << " txs/sec" << std::endl;
			m_trxCounter = 0;
			start = std::chrono::high_resolution_clock::now();
		}
	}
}

void MatchingEngine::StreamMarketData()
{
	auto streamer = MarketDataStreamer();
	std::vector<Order> orders;
	
	while (m_isProcessing)
	{
		streamer.GetData(orders);
		ReceiveMarketDataStream(orders);		
	}
}

void MatchingEngine::NotifyOrderUpdate(std::shared_ptr<OrderUpdate> orderUpdate)
{
	if (m_clientMap.count(orderUpdate->clientId) == 0)
		return;
	m_clientMap[orderUpdate->clientId]->Notify(*orderUpdate);
}

void MatchingEngine::NotifyTrade(std::shared_ptr<Trade> trade)
{
	if (m_clientMap.count(trade->clientId) == 0)
		return;
	m_clientMap[trade->clientId]->Notify(*trade);
}

