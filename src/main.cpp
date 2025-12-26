#include <iostream>
#include "matching_engine/matching_engine.hpp"

int main(){

    OrderBook testOrderBook;
    MatchingEngine testEngine;
    // Order order1(1, 100, Side::Sell, OrderType::Limit, 100);
    // std::shared_ptr<Order> order_ptr1 = std::make_shared<Order>(order1);
    auto order_ptr1 = std::make_shared<Order>(1, 100, Side::Sell, OrderType::Limit, 100);
    testEngine.ProcessOrder(order_ptr1, testOrderBook);

    testOrderBook.PrintOrderBook();
    
    // Order order2(2, 50, Side::Buy, OrderType::Market);
    // std::shared_ptr<Order> order_ptr2 = std::make_shared<Order>(order2);
    auto order_ptr2 = std::make_shared<Order>(2, 50, Side::Buy, OrderType::Market);
    testEngine.ProcessOrder(order_ptr2, testOrderBook);
    
    testOrderBook.PrintOrderBook();

    return 0;
}