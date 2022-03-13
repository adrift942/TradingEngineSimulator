#include "MarketDataStreamer.h"

void MarketDataStreamer::GetData(OrderBook& orderBook) const
{
	std::forward_list<std::deque<Order>> bids;
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f)}); // best bid

	std::forward_list<std::deque<Order>> asks;
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f) }); // best ask

	orderBook.m_bids = bids;
	orderBook.m_asks = asks;
}
