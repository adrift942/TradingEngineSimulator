#include "MatchingEngine.h"
#include "MarketDataStreamer.h"
#include <thread>


static OrderId orderIdCount = 0;


MatchingEngine::MatchingEngine()
{
	auto streamer = MarketDataStreamer();
	OrderBook orderBook;
	streamer.GetData(m_orderBook);
	m_isProcessing = true;
	m_transactionsQueue.reset(new std::deque<Transaction>());
	m_processingThread.reset(new std::thread(&MatchingEngine::ProcessingQueue, this));
}

// PUBLIC METHODS
void MatchingEngine::Stop()
{
	m_isProcessing = false;
	m_processingThread->join();
}

void MatchingEngine::InsertOrder(const ClientId& clientId, const Order& order)
{
	// assuming the client has enough balance to place the order
	const auto& client = m_clientMap[clientId];
	auto ack = Ack{ true, "Order inserted" };
	client->Notify(ack);

	// return success Ack
	AddTransactionToProcessingQueue(Transaction{ 0, order, TransactionType::Insert });
}

void MatchingEngine::AmendOrder(const ClientId& clientId, const OrderId& orderId, const Order& order)
{
	// validation
	if (m_orderBook.OrderExists(orderId))
		; // return success Ack
	else
		; // return fail Ack

	AddTransactionToProcessingQueue({ orderId, order, TransactionType::Amend });
}

void MatchingEngine::CancelOrder(const ClientId& clientId, const OrderId& orderId)
{
	// validation
	if (!m_orderBook.OrderExists(orderId))
		; // return success Ack
	else
		; // return fail Ack

	AddTransactionToProcessingQueue({ orderId, Order(), TransactionType::Cancel });
}

// PRIVATE METHODS
void MatchingEngine::AddTransactionToProcessingQueue(const Transaction& transaction)
{
	std::unique_lock<std::mutex> locker(m_mu);
	m_cv.wait(locker, []() { return true; });
	m_transactionsQueue->push_back(transaction);
	locker.unlock();
	m_cv.notify_one();
}

void MatchingEngine::ProcessingQueue()
{
	while (m_isProcessing)
	{
		std::unique_lock<std::mutex> lock(m_mu);
		if (m_transactionsQueue->empty())
			m_cv.wait_for(lock, std::chrono::milliseconds(100)); // to avoid cpu overclocking if the queue is empty
		
		if (!m_transactionsQueue->empty())
		{
			const auto& transaction = m_transactionsQueue->front();
			m_transactionsQueue->pop_front();

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
	}
}

