#include "OrderBook.h"
#include <float.h>


OrderBook::OrderBook(std::forward_list<std::deque<Order>>& bids, std::forward_list<std::deque<Order>>& asks)
{
	m_bids = bids;
	m_asks = asks;
}

// PUBLIC METHODS
float OrderBook::GetBestBidPrice() const
{
	if (m_bids.empty())
		return 0;
	else if (m_bids.front().empty())
		throw std::exception("Best bid is empty and has not been removed");
	return m_bids.front().front().price;
};

float OrderBook::GetBestAskPrice() const
{
	if (m_asks.empty())
		return FLT_MAX;
	else if (m_asks.front().empty())
		throw std::exception("Best ask is empty and has not been removed");
	return m_asks.front().front().price;
};

bool OrderBook::OrderExists(const OrderId& orderId) const
{
	return m_ordersMap.count(orderId) > 0;
}

void OrderBook::InsertOrder(const Order& i_order)
{
	auto order = i_order;

	// Execute order if it is marketable
	ExecuteOrder(order);

	// Insert order in the order book is it was not marketable or if some unfilled amount is left
	if (order.unfilledAmount > 0)
		InsertPendingOrder(order);
}

bool OrderBook::AmendOrder(const OrderId& orderId, const Order& i_order)
{
	auto res = CancelOrder(orderId);
	if (!res)
		return false;

	auto order = i_order;
	order.id = orderId;
	InsertOrder(order);

	OrderUpdate orderUpdate{ order.clientId, OrderUpdateType::Amended, order };
	Notify(orderUpdate);
	return true;
}

bool OrderBook::CancelOrder(const OrderId& orderId)
{
	if (!OrderExists(orderId))
		return false;

	const auto order = m_ordersMap[orderId];
	m_ordersMap.erase(orderId);

	if (order.isBuy)
	{
		auto it = m_bids.begin();
		auto prevIt = it;
		while (order.price != it->front().price)
		{
			prevIt = it;
			it++;
		}

		for (auto queueit = it->begin(); queueit != it->end(); queueit++)
		{
			if (queueit->id == order.id)
			{
				it->erase(queueit);
				break;
			}
		}

		if (it->empty())
			if (it == prevIt)
				m_bids.pop_front();
			else
				m_bids.erase_after(prevIt);
	}
	else
	{
		auto it = m_asks.begin();
		auto prevIt = it;
		while (order.price != it->front().price)
		{
			prevIt = it;
			it++;
		}

		for (auto queueit = it->begin(); queueit != it->end(); queueit++)
		{
			if (queueit->id == order.id)
			{
				it->erase(queueit);
				break;
			}
		}

		if (it->empty())
			if (it == prevIt)
				m_asks.pop_front();
			else
				m_asks.erase_after(prevIt);
	}


	OrderUpdate orderUpdate{ order.clientId, OrderUpdateType::Canceled, order};
	Notify(orderUpdate);
	return true;
}

void OrderBook::SetCallback(std::function<void(std::shared_ptr<OrderUpdate>)> callback)
{
	m_orderUpdateCallback = callback;
}

void OrderBook::SetCallback(std::function<void(std::shared_ptr<Trade>)> callback)
{
	m_tradeCallback = callback;
}

bool OrderBook::operator==(const OrderBook& other) const
{
	auto it1 = m_asks.begin();
	auto it2 = other.m_asks.begin();

	while (it1 != m_asks.end() && it2 != other.m_asks.end())
	{
		auto q1 = *it1;
		auto q2 = *it2;

		if (q1.size() != q2.size())
			return false;
		while (q1.size() > 0)
		{
			if (q1.front() != q2.front())
				return false;
			q1.pop_front();
			q2.pop_front();
		}

		it1++;
		it2++;
	}

	if (it1 != m_asks.end() || it2 != other.m_asks.end())
		return false;

	it1 = m_bids.begin();
	it2 = other.m_bids.begin();

	while (it1 != m_bids.end() && it2 != other.m_bids.end())
	{
		auto q1 = *it1;
		auto q2 = *it2;

		if (q1.size() != q2.size())
			return false;
		while (q1.size() > 0)
		{
			if (q1.front() != q2.front())
				return false;
			q1.pop_front();
			q2.pop_front();
		}

		it1++;
		it2++;
	}

	if (it1 != m_bids.end() || it2 != other.m_bids.end())
		return false;

	return true;
}

bool OrderBook::operator!=(const OrderBook& other) const
{
	return !(*this == other);
}

// PRIVATE METHODS
void OrderBook::ExecuteOrder(Order& order)
{
	bool executed = false;

	if (order.isBuy)
	{
		while (!m_asks.empty() && order.unfilledAmount > 0 && order.price >= GetBestAskPrice())
		{
			executed = true;
			Order& bestAskOrder = m_asks.front().front();
			float tradeAmount = std::min(order.unfilledAmount, bestAskOrder.unfilledAmount);
			order.unfilledAmount -= tradeAmount;
			bestAskOrder.unfilledAmount -= tradeAmount;

			// notify maker order
			Trade trade{ bestAskOrder.clientId, false, order.price, tradeAmount};
			Notify(trade);

			if (FloatEqual(bestAskOrder.unfilledAmount, 0))
			{
				m_asks.front().pop_front();
				if (m_asks.front().empty())
					m_asks.pop_front();
			}
		}
	}
	else
	{
		while (!m_bids.empty() && order.unfilledAmount > 0 && order.price <= GetBestBidPrice())
		{
			executed = true;
			Order& bestBidOrder = m_bids.front().front();
			float tradeAmount = std::min(order.unfilledAmount, bestBidOrder.unfilledAmount);
			order.unfilledAmount -= tradeAmount;
			bestBidOrder.unfilledAmount -= tradeAmount;

			// notify maker order
			Trade trade{ bestBidOrder.clientId, true, order.price, tradeAmount};
			Notify(trade);

			if (FloatEqual(bestBidOrder.unfilledAmount, 0))
			{
				m_bids.front().pop_front();
				if (m_bids.front().empty())
					m_bids.pop_front();
			}
		}
	}

	if (executed)
	{
		// notify taker order
		Trade trade{ order.clientId, order.isBuy, order.price, order.amount-order.unfilledAmount };
		Notify(trade);
	}
}

void OrderBook::InsertPendingOrder(Order& order)
{
	if (order.isBuy)
	{		
		if (order.price > GetBestBidPrice())
		{
			auto tmp = std::deque<Order>{ order };
			m_bids.push_front(tmp);
		}
		else
		{
			auto it = m_bids.begin();
			auto prevIt = it;
			while (it != m_bids.end() && order.price != it->front().price)
			{
				prevIt = it;
				it++;
			}

			if (prevIt == it)
			{
				m_bids.front().push_back(order);
			}
			else if (it != m_bids.end() && FloatEqual(it->front().price, order.price))
			{
				it->push_back(order);
			}
			else
			{
				auto tmp = std::deque<Order>{ order };
				m_bids.insert_after(prevIt, tmp);
			}
		}		
	}
	else
	{
		if (order.price < GetBestAskPrice())
		{
			auto tmp = std::deque<Order>{ order };
			m_asks.push_front(tmp);
		}
		else
		{
			auto it = m_asks.begin();
			auto prevIt = it;
			while (it != m_asks.end() && order.price > it->front().price)
			{
				prevIt = it;
				it++;
			}
			if (prevIt == it)
			{
				m_asks.front().push_back(order);
			}
			else if (it != m_asks.end() && FloatEqual(it->front().price, order.price))
			{
				it->push_back(order);
			}
			else
			{
				auto tmp = std::deque<Order>{ order };
				m_asks.insert_after(prevIt, tmp);
			}
		}
	}

	m_ordersMap[order.id] = order;
	OrderUpdate orderUpdate{ order.clientId, OrderUpdateType::Open, order };
	Notify(orderUpdate);
}

void OrderBook::Notify(const OrderUpdate& orderUpdate)
{
	if (m_isTest)
		return;
	auto o = std::make_shared<OrderUpdate>(orderUpdate);
	m_orderUpdateCallback(o);
}

void OrderBook::Notify(const Trade& trade)
{
	if (m_isTest)
		return;
	auto t = std::make_shared<Trade>(trade);
	m_tradeCallback(t);
}
