#pragma once
#include <deque>
#include <forward_list>
#include <algorithm>
#include "Order.h"



struct OrderBook
{
	std::forward_list<std::deque<Order>> bids;
	std::forward_list<std::deque<Order>> asks;

	float GetBestBidPrice() { return bids.front().front().price; }
	float GetBestAskPrice() { return asks.front().front().price; }

	void InsertOrder(const Order& i_order)
	{
		auto order = i_order;
		if (order.isBuy)
		{
			while (order.unfilledAmount > 0 && order.price >= GetBestAskPrice())
			{
				float tradeAmount = std::min(order.unfilledAmount, asks.front().front().unfilledAmount);
				order.unfilledAmount -= tradeAmount;
				asks.front().front().unfilledAmount -= tradeAmount;

				if (asks.front().front().unfilledAmount == 0)
				{
					asks.front().pop_front();
					if (asks.front().empty())
					{
						asks.pop_front();
					}
				}
			}

			if (order.unfilledAmount > 0)
			{
				if (order.price > GetBestBidPrice())
				{
					auto tmp = std::deque<Order>{ order };
					bids.push_front(tmp);
				}
				else if (order.price == GetBestBidPrice())
				{
					bids.front().push_back(order);
				}
				else
				{
					auto it = bids.begin();
					auto prevIt = it;
					//auto frontPrice = (*it).front().price;
					while (order.price < (*it).front().price)
					{
						prevIt = it;
						it++;
						//frontPrice = (*it).front().price;
					}
					if ((*prevIt).front().price == order.price)
					{
						(*prevIt).push_back(order);
					}
					else
					{
						auto tmp = std::deque<Order>{ order };
						bids.insert_after(prevIt, tmp);
					}
				}
			}
		}
		else
		{

		}

		// if there is some unfilled quantity
		//if (order.unfilledAmount > 0)
		//	insert(std::pair<OrderId, Order>(order.id, order));
	}

	bool operator==(OrderBook& other) const
	{
		auto it1 = asks.begin();
		auto it2 = other.asks.begin();

		while (it1 != asks.end() && it2 != other.asks.end())
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

		if (it1 != asks.end() || it2 != other.asks.end())
			return false;

		return true;
	}
};