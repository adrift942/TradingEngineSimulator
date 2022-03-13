#include "MatchingEngine.h"
#include "Client.h"
#include <iostream>
#include <memory>

int main()
{
    auto engine = std::make_shared<MatchingEngine>();
    auto client = Client(engine);
    engine->SubscribeClient(client.id, std::make_shared<Client>(client));

    client.InsertOrder(Order(true, 1, 1));
}