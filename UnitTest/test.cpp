#include "pch.h"
#include "OrderBook.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

struct OrderBookTest : testing::Test {

	OrderBookTest() {

		std::forward_list<std::deque<Order>> bids;
		bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
		bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f )});
		bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f )});
		bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f )});
		bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f )});

		std::forward_list<std::deque<Order>> asks;
		asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
		asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
		asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
		asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});
		asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f) });

		orderBook.bids = bids; 
		orderBook.asks = asks;
	}

	OrderBook orderBook;
};


TEST_F(OrderBookTest, Buy1) {
	
	auto order = Order(true, 10.02f, 5000.f);
	orderBook.InsertOrder(order);
	EXPECT_FLOAT_EQ(orderBook.asks.front().front().price, 10.02f);
	EXPECT_FLOAT_EQ(orderBook.asks.front().front().unfilledAmount, 5000.f);
}

TEST_F(OrderBookTest, Buy2) {

	auto order = Order(true, 10.02f, 12000.f);
	orderBook.InsertOrder(order);

	OrderBook res;
	std::forward_list<std::deque<Order>> bids;
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.02f, 2000.f)});
	std::forward_list<std::deque<Order>> asks;
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});
	res.bids = bids;
	res.asks = asks;

	EXPECT_TRUE(orderBook == res);
}

//TEST_F(OrderBookTest, Buy3) {
//
//	auto order = Order(true, 10.04f, 12000.f);
//	orderBook.InsertOrder(order);
//	EXPECT_FLOAT_EQ(orderBook.asks.front().front().price, 10.04f);
//	EXPECT_FLOAT_EQ(orderBook.asks.front().front().unfilledAmount, 6000.f);
//}
