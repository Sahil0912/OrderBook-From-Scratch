#include "matching_engine/matching_engine.hpp"
#include "utils/memory_pool.hpp"

int main(){

    OrderBook testOrderBook;
    MatchingEngine testEngine;
    MemoryPool<Order, 1024> pool;
    Order* order1 = pool.emplace(1, 100, Side::Sell, OrderType::Limit, 100);
    testEngine.ProcessOrder(order1, testOrderBook);
    Order* order2 = pool.emplace(2, 101, Side::Buy, OrderType::Limit, 101);
    testEngine.ProcessOrder(order2, testOrderBook);
    testOrderBook.PrintOrderBook();

    return 0;
}