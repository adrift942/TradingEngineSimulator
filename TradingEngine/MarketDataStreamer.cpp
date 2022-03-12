#include "MarketDataStreamer.h"

OrderBook& MarketDataStreamer::GetData() const
{
    std::deque<PendingOrder> bids;
    bids.push_back(PendingOrder{ 10.f, 200.f });
    bids.push(PendingOrder{ 9.99f, 8000.f });
    bids.push(PendingOrder{ 9.98f, 3000.f });
    bids.push(PendingOrder{ 9.97f, 2000.f });
    bids.push(PendingOrder{ 9.96f, 40000.f });

    std::deque<PendingOrder> asks;
    asks.push(PendingOrder{ 10.02f, 10000.f });
    asks.push(PendingOrder{ 10.04f, 8000.f });
    asks.push(PendingOrder{ 10.06f, 6000.f });
    asks.push(PendingOrder{ 10.08f, 20000.f });
    asks.push(PendingOrder{ 10.10f, 500.f });

    auto orderBook = OrderBook{ bids, asks };
    return orderBook;
}
