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

    uint64_t count = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < 100000; i++)
    {
        ack = client1.InsertOrder(Order(true, Randomize(10.02f), Randomize(5000.f)));
        count++;
        client2.InsertOrder(Order(false, Randomize(10.21f), Randomize(6000.f)));
        count++;
        client1.CancelOrder(ack.orderId);
        client3.InsertOrder(Order(true, Randomize(9.91f), Randomize(1000.f)));
        count++;
        ack = client3.InsertOrder(Order(true, Randomize(3.81f), Randomize(1000.f)));
        count++;
        client3.CancelOrder(ack.orderId);
        count++;
        
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        if (duration.count() >= 1000)
        {
            std::cout << "Processed " << count << " txs/sec" << std::endl;
            count = 0;
            start = std::chrono::high_resolution_clock::now();
        }
    }       

    std::cout << "\nDone" << std::endl;
}