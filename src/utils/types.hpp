#pragma once

#include <cstdint>

using Price = uint64_t;
using Quantity = uint64_t;
using OrderID = uint64_t;

enum class Side : uint8_t{
    Buy, Sell
};

enum class OrderType : uint8_t{
    Market, Limit, FOK, IOC
};
