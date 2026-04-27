#pragma once
#include "../order_book/order_book.hpp"

class MatchingEngine{
    private :
    public :
        void ProcessOrder(Order* order, OrderBook &orderbook);
};