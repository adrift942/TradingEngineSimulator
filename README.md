# TradingEngineSimulator

This is a trading matching engine written in C++. The solution is composed of 3 projects:

- **TradingEngine**: static library with all the defitions of the classes. In particular: MatchingEngine, OrderBook, Client
- **TestApp**: a test application to run the engine and measure its performance in terms of transactions per second
- **UnitTest**: to test the most relevant algorithms

All the orders are limit and are inserted in the order book following a price/time priority. The algorithms that handle the execution and insertion of the orders are defined in the OrderBook class.
The OrderBook is structured in a way that maximizes the number of transactions per second, assuming that most orders will be inserted around the mark price.
