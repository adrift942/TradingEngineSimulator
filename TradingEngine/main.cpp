#include "MatchingEngine.h"
#include "Client.h"
#include <iostream>
#include <memory>

using namespace std::chrono_literals;

int main()
{
    auto engine = std::make_shared<MatchingEngine>();
    auto client = Client(engine);
    engine->SubscribeClient(client.id, std::make_shared<Client>(client));

    client.InsertOrder(Order(true, 10.02f, 5000.f));

    std::this_thread::sleep_for(2000ms);
    std::cout << "\nDone" << std::endl;
}