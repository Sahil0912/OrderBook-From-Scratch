#pragma once
#include "../order_book/order_book.hpp"
#include "../utils/memory_pool.hpp"
#include "../utils/order_lookup.hpp"

class MatchingEngine{
    private :
        OrderBook book_;
        MemoryPool<Order, 100000> pool_; //will see later about what to do with the slots capacity
        OrderLookup<100000> lookup_;
    public :
        explicit MatchingEngine(Price basePrice);
        void ProcessOrder(Order* order);
        void ProcessOrder(OrderID id, Quantity qty, Side side, OrderType type, Price price = 0);
        bool CancelOrder(OrderID id);
        bool CancelOrder(Order* order);
        void PrintOrderBook() const;
};