#include "order_book.hpp"
#include <iostream>

void OrderBook::AddOrder(Order* order){
    if((order->side) == Side::Buy){
        //using the emplace to 
        bids.emplace(order->price, PriceLevel(order->price));
        bids.at(order->price).AddOrder(order);
    }
    else{
        asks.emplace(order->price, PriceLevel(order->price));
        asks.at(order->price).AddOrder(order);
    }
}

std::map<Price, PriceLevel >& OrderBook::GetAsks(){
    return asks;
}

std::map<Price, PriceLevel, std::greater<Price> >& OrderBook::GetBids(){
    return bids;
}

// bool OrderBook::IsEmptyBids(){
//     return bids.empty();
// }

// bool OrderBook::IsEmptyAsks(){
//     return asks.empty();
// }

void OrderBook::RemoveAskLevel(Price price){
    if(asks.find(price) == asks.end()) return;
    asks.erase(price);
}

void OrderBook::RemoveBidLevel(Price price){
    if(bids.find(price) == bids.end()) return;
    bids.erase(price);
}

void OrderBook::PrintOrderBook(){
    std::cout << "---------Bids---------\n\n";
    for(auto &[price, pricelevel] : bids){
        std::cout << "Price : " << price << "\n";
        pricelevel.PrintPriceLevel();
    }
    std::cout << "\n\n";
    std::cout << "---------Asks---------\n\n";
    for(auto &[price, pricelevel] : asks){
        std::cout << "Price : " << price << "\n";
        pricelevel.PrintPriceLevel();
    }
    std::cout << "\n\n";
}