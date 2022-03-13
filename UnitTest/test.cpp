#include "gtest/gtest.h"
#include "OrderBook.h"


struct OrderBookTest : testing::Test {

	OrderBookTest() {
		orderBook.SetTest(true);

		std::forward_list<std::deque<Order>> bids;
		bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
		bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f )});
		bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f )});
		bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f )});
		bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f )}); // best bid

		std::forward_list<std::deque<Order>> asks;
		asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
		asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
		asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
		asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});
		asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f) }); // best ask

		orderBook.m_bids = bids;
		orderBook.m_asks = asks;
	}

	OrderBook orderBook;
};


TEST_F(OrderBookTest, Buy1)
{	
	auto order = Order(true, 10.02f, 5000.f);
	orderBook.InsertOrder(order);
	EXPECT_FLOAT_EQ(orderBook.GetBestAskPrice(), 10.02f);
	EXPECT_FLOAT_EQ(orderBook.GetBestAskAmount(), 5000.f);

	order = Order(false, 10.0f, 100.f);
	orderBook.InsertOrder(order);
	EXPECT_FLOAT_EQ(orderBook.GetBestBidPrice(), 10.0f);
	EXPECT_FLOAT_EQ(orderBook.GetBestBidAmount(), 100.f);
}

TEST_F(OrderBookTest, Buy2)
{
	auto order = Order(true, 10.02f, 12000.f);
	orderBook.InsertOrder(order);

	order = Order(false, 10.f, 500);
	orderBook.InsertOrder(order);

	OrderBook res;
	std::forward_list<std::deque<Order>> bids;
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.02f, 1500.f)});
	std::forward_list<std::deque<Order>> asks;
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});

	res.m_bids = bids;
	res.m_asks = asks;

	EXPECT_TRUE(orderBook == res);
}

TEST_F(OrderBookTest, Buy3)
{
	auto order = Order(true, 10.04f, 12000.f);
	orderBook.InsertOrder(order);
	EXPECT_FLOAT_EQ(orderBook.GetBestAskPrice(), 10.04f);
	EXPECT_FLOAT_EQ(orderBook.GetBestAskAmount(), 6000.f);
}

TEST_F(OrderBookTest, CancelOrder1)
{
	auto order = Order(true, 9.98f, 12000.f);
	order.id = 1;
	orderBook.InsertOrder(order);
	auto success = orderBook.CancelOrder(1);
	EXPECT_TRUE(success);

	order = Order(false, 10.05f, 2000.f);
	order.id = 2;
	orderBook.InsertOrder(order);
	success = orderBook.CancelOrder(2);
	EXPECT_TRUE(success);

	OrderBook res;
	std::forward_list<std::deque<Order>> bids;
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f)});

	std::forward_list<std::deque<Order>> asks;
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f) });

	res.m_bids = bids;
	res.m_asks = asks;

	EXPECT_TRUE(orderBook == res);
}