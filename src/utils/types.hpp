#pragma once

#include <cstdint>
#include <cstddef>
constexpr size_t MAX_PRICE_LEVELS = 100000; 

using Price = uint64_t;
using Quantity = uint64_t;
using OrderID = uint64_t;

enum class Side : uint8_t{
    Buy, Sell
};

enum class OrderType : uint8_t{
    Market, Limit, FOK, IOC
};
