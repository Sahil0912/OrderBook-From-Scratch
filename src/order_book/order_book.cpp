#include "order_book.hpp"
#include "order.hpp"
#include "price_level.hpp"
#include <iostream>

void OrderBook::AddOrder(Order* order){
    size_t idx = order->price - basePrice_;
    PriceLevel& currLevel = OrderBook::GetLevel(order->price);

    if(order->side == Side::Buy){
        if(bestBidPrice_ < order->price){
            bestBidPrice_ = order->price;
        }
    } else {
        if(bestAskPrice_ > order->price){
            bestAskPrice_ = order->price;
        }
    }

    currLevel.AddOrder(order);
    SetBit(idx); //marking this idx as alive
}

void OrderBook::RemoveOrder(Order* order){
    auto& currLevel = OrderBook::GetLevel(order->price);
    if(!currLevel.IsEmpty()){
        currLevel.RemoveOrder(order);
    }

    //if level is now empty, clear bitmap bit and recalc best price if needed
    if(currLevel.IsEmpty()){
        ClearBit(order->price - basePrice_);

        if(order->side == Side::Buy && order->price == bestBidPrice_){
            bestBidPrice_ = FindNextBid(order->price);
        }
        if(order->side == Side::Sell && order->price == bestAskPrice_){
            bestAskPrice_ = FindNextAsk(order->price);
        }
    }
}

//finding next non-empty ask level at or above from
//skipping 64 levels per iter
Price OrderBook::FindNextAsk(Price from) const {
    size_t idx = from - basePrice_;
    size_t word = idx / 64;
    size_t bit  = idx % 64;

    //checking per word if there is any ask alive
    uint64_t masked = bitmap_[word] >> bit;
    if(masked){
        return basePrice_ + word * 64 + bit + __builtin_ctzll(masked);
    }

    for(word++; word < BITMAP_WORDS; word++){
        if(bitmap_[word]){
            return basePrice_ + word * 64 + __builtin_ctzll(bitmap_[word]);
        }
    }
    //incase no ask found
    return basePrice_ + MAX_PRICE_LEVELS;
}

Price OrderBook::FindNextBid(Price from) const {
    if(from < basePrice_) return 0;

    size_t idx = from - basePrice_;
    size_t word = idx / 64;
    size_t bit  = idx % 64;

    uint64_t masked = bitmap_[word] & ((2ULL << bit) - 1);
    if(masked){
        return basePrice_ + word * 64 + 63 - __builtin_clzll(masked);
    }

    for(int w = (int)word - 1; w >= 0; w--){
        if(bitmap_[w]){
            return basePrice_ + w * 64 + 63 - __builtin_clzll(bitmap_[w]);
        }
    }
    return 0;
}

void OrderBook::PrintOrderBook() const{
    std::cout << "---------Bids---------\n\n";
    if(bestBidPrice_ != 0){
        for(int i = bestBidPrice_ - basePrice_; i >= 0 ; i--){
            const PriceLevel& pricelevel = OrderBook::GetLevel(basePrice_ + i);
            if(!pricelevel.IsEmpty()){
                std::cout << "Price : " << pricelevel.GetPrice() << "\n";
                pricelevel.PrintPriceLevel();
                std::cout << "--------------\n";
                std::cout << "\n";
            }
            
        }
    }
    
    std::cout << "\n\n";
    std::cout << "---------Asks---------\n\n";
    for(int i = bestAskPrice_ - basePrice_; i < MAX_PRICE_LEVELS; i++){
        
        const PriceLevel& pricelevel = OrderBook::GetLevel(basePrice_ + i);
        if(!pricelevel.IsEmpty()){
            std::cout << "Price : " << pricelevel.GetPrice() << "\n";
            pricelevel.PrintPriceLevel();
            std::cout << "--------------\n";
            std::cout << "\n";
        }
    }
    std::cout << "\n\n";
}