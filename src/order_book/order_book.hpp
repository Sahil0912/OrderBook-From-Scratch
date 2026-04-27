#pragma once
#include "price_level.hpp"
#include <map>

class OrderBook{
    private : 
        std::map<Price, PriceLevel, std::greater<Price> > bids;
        std::map<Price, PriceLevel > asks;
    public:
        void AddOrder(Order* order);
        std::map<Price, PriceLevel >& GetAsks();
        std::map<Price, PriceLevel, std::greater<Price> >& GetBids();
        void RemoveAskLevel(Price);
        void RemoveBidLevel(Price);
        // bool IsEmptyBids();
        // bool IsEmptyAsks();
        void PrintOrderBook();
    // we will still be able to change the content of PriceLevel

};