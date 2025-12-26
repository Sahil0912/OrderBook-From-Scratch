#pragma once

#include "../utils/types.hpp"

struct Order
{
    OrderID ID;
    Price price;
    Quantity quantity;
    Quantity init_quantity;
    Side side;
    OrderType type;

    Order(OrderID _ID, Quantity _init_quantity, Side _side, OrderType _type, Price _price = 0) :
        ID(_ID), quantity(_init_quantity), init_quantity(_init_quantity), side(_side), type(_type), price(_price)
    {}

    void PrintOrder();
};