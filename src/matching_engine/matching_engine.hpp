#pragma once
#include "../order_book/order_book.hpp"

class MatchingEngine{
    private :
    public :
        void ProcessOrder(std::shared_ptr<Order> order, OrderBook &orderbook);
};