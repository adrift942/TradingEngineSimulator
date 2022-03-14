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

    client1.InsertOrder(Order(true, 10.01f, 5000.f));
    client2.InsertOrder(Order(false, 10.01f, 6000.f));
    client3.InsertOrder(Order(true, 10.01f, 1000.f));

    std::this_thread::sleep_for(10s);
    std::cout << "\nDone" << std::endl;
}