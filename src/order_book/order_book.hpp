#pragma once
#include "price_level.hpp"

class OrderBook {
private:
    const Price basePrice_;
    PriceLevel levels_[MAX_PRICE_LEVELS];
    Price bestBidPrice_;
    Price bestAskPrice_;

public:
    explicit OrderBook(Price basePrice) : basePrice_(basePrice), bestBidPrice_(0), bestAskPrice_(basePrice_ + MAX_PRICE_LEVELS - 1) 
    {
        for(size_t i = 0; i < MAX_PRICE_LEVELS; ++i){
            levels_[i] = PriceLevel(basePrice_ + i);
        }
    }

    //O(1)
    PriceLevel& GetLevel(Price price) { return levels_[price - basePrice_]; }
    const PriceLevel& GetLevel(Price price) const { return levels_[price - basePrice_]; } //for reading

    Price GetBasePrice() const { return basePrice_; }
    Price GetBestBidPrice() const { return bestBidPrice_; }
    Price GetBestAskPrice() const { return bestAskPrice_; }
    
    void SetBestBidPrice(Price price) { bestBidPrice_ = price; }
    void SetBestAskPrice(Price price) { bestAskPrice_ = price; }

    void AddOrder(Order* order);

    void RemoveOrder(Order *order);
    void ReCalcBestBidAndAsk();

    void PrintOrderBook() const;
};