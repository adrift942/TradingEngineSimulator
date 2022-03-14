#include "MatchingEngine.h"
#include "Client.h"
#include <iostream>
#include <memory>

using namespace std::chrono_literals;

int main()
{
    auto engine = std::make_shared<MatchingEngine>();
    auto client1 = Client(engine);
    auto client2 = Client(engine);
    auto client3 = Client(engine);

    Ack ack;

    ack = client1.InsertOrder(Order(true, 2.f, 500.f));
    ack = client1.CancelOrder(ack.orderId);
    std::cout << "Client ID " << client1.id << " response from server: " << ack << std::endl;

    ack = client2.InsertOrder(Order(false, 33.f, 900.f));
    ack = client2.AmendOrder(ack.orderId, Order(false, 33.f, 900.f));
    std::cout << "Client ID " << client2.id << " response from server: " << ack << std::endl;

    for (auto i = 0; i < 10; i++)
    {
        ack = client1.InsertOrder(Order(true, 10.01f, 5000.f));
        std::cout << "Client ID " << client1.id << " response from server: " << ack << std::endl;

        ack = client2.InsertOrder(Order(false, 10.01f, 6000.f));
        std::cout << "Client ID " << client1.id << " response from server: " << ack << std::endl;

        ack = client3.InsertOrder(Order(true, 10.01f, 1000.f));
        std::cout << "Client ID " << client1.id << " response from server: " << ack << std::endl;        
        
        std::this_thread::sleep_for(1s);
    }       

    std::cout << "\nDone" << std::endl;
}