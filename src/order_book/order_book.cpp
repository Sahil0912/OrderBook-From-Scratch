#include "order_book.hpp"
#include "order.hpp"
#include "price_level.hpp"
#include <iostream>

void OrderBook::AddOrder(Order* order){
    if((order->side) == Side::Buy){
        if(OrderBook::GetBestBidPrice() < order->price){
            OrderBook::SetBestBidPrice(order->price);
        }
        PriceLevel& currLevel = OrderBook::GetLevel(order->price);
        currLevel.AddOrder(order);
    }
    else{
        if(OrderBook::GetBestAskPrice() > order->price){
            OrderBook::SetBestAskPrice(order->price);
        }
        PriceLevel& currLevel = OrderBook::GetLevel(order->price);
        currLevel.AddOrder(order);
    }
}

void OrderBook::PrintOrderBook() const{
    std::cout << "---------Bids---------\n\n";
    if(bestBidPrice_ != 0){
        for(int i = bestBidPrice_ - basePrice_; i >= 0 ; i--){
            const PriceLevel& pricelevel = OrderBook::GetLevel(basePrice_ + i);
            if(!pricelevel.IsEmpty()){
                std::cout << "Price : " << pricelevel.GetPrice() << "\n";
                pricelevel.PrintPriceLevel();
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
        }
    }
    std::cout << "\n\n";
}