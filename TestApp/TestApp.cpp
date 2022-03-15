#include "MatchingEngine.h"
#include "Client.h"
#include "Util.h"
#include <iostream>
#include <memory>
#include <chrono>

using namespace std::chrono_literals;

int main()
{
    auto engine = std::make_shared<MatchingEngine>();
    
    auto client1 = Client(engine, true);
    auto client2 = Client(engine, true);    

    Ack ack;

    // Test Ack responses, order update and trade notifications
    ack = client1.InsertOrder(Order(true, 2.f, 500.f));
    std::cout << "Client ID " << client1.id << " response from server: " << ack << std::endl;
    ack = client1.CancelOrder(ack.orderId);
    std::cout << "Client ID " << client1.id << " response from server: " << ack << std::endl;
    ack = client2.InsertOrder(Order(false, 33.f, 900.f));
    std::cout << "Client ID " << client1.id << " response from server: " << ack << std::endl;
    ack = client2.AmendOrder(ack.orderId, Order(false, 33.f, 900.f));
    std::cout << "Client ID " << client2.id << " response from server: " << ack << std::endl;



    // Test number of transactions per second
    auto client3 = Client(engine);
    auto client4 = Client(engine);
    auto client5 = Client(engine);
    for (auto i = 0; i < 100000; i++)
    {
        client3.InsertOrder(Order(true, Randomize(10.02f), Randomize(5000.f)));
        client4.InsertOrder(Order(false, Randomize(10.21f), Randomize(6000.f)));
        client5.InsertOrder(Order(true, Randomize(9.91f), Randomize(1000.f)));
        ack = client5.InsertOrder(Order(true, Randomize(3.81f), Randomize(1000.f)));
        client5.CancelOrder(ack.orderId);
    }       

    std::cout << "\nDone" << std::endl;
}