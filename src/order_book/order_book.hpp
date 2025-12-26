#pragma once
#include "price_level.hpp"
#include <map>
#include <functional>

class OrderBook{
    private : 
        std::map<Price, std::shared_ptr<PriceLevel>, std::greater<Price> > bids;
        std::map<Price, std::shared_ptr<PriceLevel> > asks;
    //making them shared to avoid accidental deep copies
    public:
        void AddOrder(std::shared_ptr<Order> order);
        const std::map<Price, std::shared_ptr<PriceLevel> >& GetAsks() const;
        const std::map<Price, std::shared_ptr<PriceLevel>, std::greater<Price> >& GetBids() const;
        void RemoveAskLevel(Price);
        void RemoveBidLevel(Price);
        // bool IsEmptyBids();
        // bool IsEmptyAsks();
        void PrintOrderBook();
    // we will still be able to change the content of PriceLevel
    

};