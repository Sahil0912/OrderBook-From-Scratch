#pragma once

#include <cstddef>
#include <cstring>
#include "../order_book/order.hpp"
#include "types.hpp"

template<std::size_t MAX_ORDERS>
class OrderLookup{
    private:
        Order* table_[MAX_ORDERS];
    public:
        OrderLookup(){
            memset(table_, 0, sizeof(table_));
        }
        void Insert(Order* order){
            table_[order->ID] = order;
        }
        void Remove(OrderID id){
            table_[id] = nullptr;
        }
        Order* Find(OrderID id){
            return table_[id];
        }
};