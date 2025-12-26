#pragma once
#include <memory>
#include <deque>
#include "order.hpp"

class PriceLevel{
    private : 
        Price price;
        std::deque<std::shared_ptr<Order>> orders;
    //making them shared to make sure when everthing gets it work done then only we will delete it
    public : 
        explicit PriceLevel(Price _price) : price(_price) {}
        Price GetPrice() const{ return price; }
        bool IsEmpty() const{ return orders.empty(); }
        const std::deque<std::shared_ptr<Order>> &GetOrders() const { return orders; }
        void AddOrder(std::shared_ptr<Order> order) { orders.push_back(order); }
        void RemoveTopOrder() { orders.pop_front(); }
        // void AddFrontOrder(std::shared_ptr<Order> order) { orders.push_front(order); }
        void PrintPriceLevel();

};