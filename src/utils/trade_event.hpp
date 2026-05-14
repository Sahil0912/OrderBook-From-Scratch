#pragma once
#include "types.hpp"
#include <cstdint>

struct TradeEvent{
    OrderID _aggressorID;
    OrderID _restingID;
    Price _price;
    Quantity _quantity;
    Side _aggressorSide;
    uint64_t timestamp;

};