#include "gtest/gtest.h"
#include "OrderBook.h"


struct OrderBookTest : testing::Test {

	OrderBookTest() {
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

		orderBook = OrderBook(bids, asks);
		orderBook.SetIsTest(true);
	}

	OrderBook orderBook;
};


TEST_F(OrderBookTest, Insert1)
{	
	orderBook.InsertOrder(Order(true, 9.98f, 2000.f));
	orderBook.InsertOrder(Order(true, 10.f, 5000.f));
	orderBook.InsertOrder(Order(true, 9.98f, 3000.f));

	orderBook.InsertOrder(Order(false, 10.06f, 100.f));
	orderBook.InsertOrder(Order(false, 10.04f, 3100.f));

	std::forward_list<std::deque<Order>> bids;
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f), Order(true, 9.98f, 2000.f), Order(true, 9.98f, 3000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f), Order(true, 10.f, 5000.f)});
	std::forward_list<std::deque<Order>> asks;
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f), Order(false, 10.06f, 100.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f), Order(false, 10.04f, 3100.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f) });
	OrderBook res(bids, asks);

	EXPECT_EQ(orderBook, res);
}

TEST_F(OrderBookTest, Insert2)
{
	auto order = Order(true, 10.02f, 12000.f);
	orderBook.InsertOrder(order);

	order = Order(false, 10.f, 500);
	orderBook.InsertOrder(order);

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
	OrderBook res(bids, asks);

	EXPECT_TRUE(orderBook == res);
}

TEST_F(OrderBookTest, Insert3)
{
	orderBook.InsertOrder(Order(true, 9.95f, 1000.f));
	orderBook.InsertOrder(Order(false, 10.11f, 1000.f));

	std::forward_list<std::deque<Order>> bids;
	bids.push_front(std::deque<Order>{Order(true, 9.95f, 1000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f)});
	std::forward_list<std::deque<Order>> asks;
	asks.push_front(std::deque<Order>{Order(false, 10.11f, 1000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f)});
	OrderBook res(bids, asks);

	EXPECT_EQ(orderBook, res);
}

TEST_F(OrderBookTest, CancelOrder1)
{
	Order order;
	bool success;

	order = Order(true, 9.98f, 12000.f);
	order.id = 100;
	orderBook.InsertOrder(order);
	success = orderBook.CancelOrder(100);
	EXPECT_TRUE(success);

	order = Order(false, 10.04f, 2000.f);
	order.id = 101;
	orderBook.InsertOrder(order);
	success = orderBook.CancelOrder(101);
	EXPECT_TRUE(success);

	std::forward_list<std::deque<Order>> bids;
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f), Order(true, 9.98f, 0.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f)});
	std::forward_list<std::deque<Order>> asks;
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f), Order(false, 10.04f, 0.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f) });
	OrderBook res(bids, asks);

	EXPECT_TRUE(orderBook == res);
}

TEST_F(OrderBookTest, CancelOrder2)
{
	Order order;
	bool success;

	order = Order(true, 9.98f, 12000.f);
	order.id = 100;
	orderBook.InsertOrder(order);
	orderBook.InsertOrder(Order(true, 9.98f, 5000.f));
	orderBook.InsertOrder(Order(true, 9.99f, 1000.f));
	
	order = Order(false, 10.05f, 2000.f);
	order.id = 101;
	orderBook.InsertOrder(order);
			
	std::forward_list<std::deque<Order>> bids;
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f), Order(true, 9.98f, 12000.f), Order(true, 9.98f, 5000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f), Order(true, 9.99f, 1000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f)});
	std::forward_list<std::deque<Order>> asks;
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.05f, 2000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f) });
	OrderBook res(bids, asks);

	EXPECT_TRUE(orderBook == res);

	// cancel orders
	success = orderBook.CancelOrder(100);
	EXPECT_TRUE(success);
	success = orderBook.CancelOrder(101);
	EXPECT_TRUE(success);

	bids.clear();
	bids.push_front(std::deque<Order>{Order(true, 9.96f, 40000.f) });
	bids.push_front(std::deque<Order>{Order(true, 9.97f, 2000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.98f, 3000.f), Order(true, 9.98f, 5000.f)});
	bids.push_front(std::deque<Order>{Order(true, 9.99f, 8000.f), Order(true, 9.99f, 1000.f)});
	bids.push_front(std::deque<Order>{Order(true, 10.f, 200.f)});
	asks.clear();
	asks.push_front(std::deque<Order>{Order(false, 10.10f, 500.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.08f, 20000.f) });
	asks.push_front(std::deque<Order>{Order(false, 10.06f, 6000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.04f, 8000.f)});
	asks.push_front(std::deque<Order>{Order(false, 10.02f, 10000.f) });
	res = OrderBook(bids, asks);

	EXPECT_TRUE(orderBook == res);
}