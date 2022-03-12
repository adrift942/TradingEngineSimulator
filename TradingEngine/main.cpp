#include <iostream>
#include "MatchingEngine.h"
#include "Client.h"
#include "Simulator.h"

int main()
{
    auto engine = MatchingEngine();
    auto client = Client();
    auto simulator = Simulator(engine, client);
    simulator.Start();
}