#pragma once
#include "price_level.hpp"
#include <cstring>

//number of 64-bit words needed for the bitmap
constexpr size_t BITMAP_WORDS = (MAX_PRICE_LEVELS + 63) / 64;

class OrderBook {
private:
    const Price basePrice_;
    PriceLevel levels_[MAX_PRICE_LEVELS];
    uint64_t bitmap_[BITMAP_WORDS];  //1 bit per price level
    Price bestBidPrice_;
    Price bestAskPrice_;

    //bitmap helpers(defining them here as compiler will treat them as inline)
    void SetBit(size_t idx) { bitmap_[idx / 64] |=  (1ULL << (idx % 64)); }
    void ClearBit(size_t idx) { bitmap_[idx / 64] &= ~(1ULL << (idx % 64)); }
    bool TestBit(size_t idx) const { return bitmap_[idx / 64] & (1ULL << (idx % 64)); }

public:
    explicit OrderBook(Price basePrice) : basePrice_(basePrice), bestBidPrice_(0), bestAskPrice_(basePrice_ + MAX_PRICE_LEVELS - 1) 
    {
        std::memset(bitmap_, 0, sizeof(bitmap_));
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

    //find next non-empty level using __builtin_ctzll
    Price FindNextAsk(Price from) const;
    Price FindNextBid(Price from) const;

    void PrintOrderBook() const;
};