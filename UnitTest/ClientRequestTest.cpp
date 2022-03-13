#include "gtest/gtest.h"
#include "Client.h"
#include "MatchingEngine.h"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;


struct ClientRequestTest : testing::Test {

	ClientRequestTest() {
		engine = std::make_shared<MatchingEngine>();
		engine->SetTest(true);
	}

	std::shared_ptr<MatchingEngine> engine;
};


//TEST_F(ClientRequestTest, InsertOrder1)
//{	
//	auto client1 = Client(engine);
//	engine->SubscribeClient(client1.id, std::make_shared<Client>(client1));
//	auto client2 = Client(engine);
//	auto client3 = Client(engine);
//
//	Ack ack;
//
//	client1.InsertOrder(Order(true, 10.01f, 5000.f));	
//	client1.GetLastAck(ack);
//	EXPECT_TRUE(ack.success);
//	
//	client2.InsertOrder(Order(false, 10.01f, 6000.f));
//	client2.GetLastAck(ack);
//	EXPECT_TRUE(ack.success);
//
//	client3.InsertOrder(Order(true, 10.01f, 1000.f));
//	client3.GetLastAck(ack);
//	EXPECT_TRUE(ack.success);
//
//	std::this_thread::sleep_for(2000ms);
//}
