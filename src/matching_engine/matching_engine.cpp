#include "matching_engine.hpp"

void MatchingEngine::ProcessOrder(Order* order, OrderBook &orderbook){
    if(order->side == Side::Buy){
        auto& asks = orderbook.GetAsks();
        auto it = asks.begin();
        while(order->quantity > 0 && it != asks.end()){
            auto& CurrPriceLevel = it->second;
            if(order->type == OrderType::Limit && CurrPriceLevel.GetPrice() > order->price){
                break;
            }
            while(order->quantity > 0 && !CurrPriceLevel.IsEmpty()){
                Order* front = CurrPriceLevel.GetHead();
                Quantity MinQuantity = std::min(front->quantity, order->quantity);
                order->quantity -= MinQuantity;
                front->quantity -= MinQuantity;
                if(front->quantity == 0){
                    CurrPriceLevel.RemoveTopOrder();
                }
            }
            if(CurrPriceLevel.IsEmpty()){
                it = asks.erase(it);
            }
            else{
                it++;
            }
        }
        if(order->quantity > 0 && order->type == OrderType::Limit){
            orderbook.AddOrder(order);
        }
    }
    else{
        auto& bids = orderbook.GetBids();
        auto it = bids.begin();
        while(order->quantity > 0 && it != bids.end()){
            auto& CurrPriceLevel = it->second;
            if(order->type == OrderType::Limit && CurrPriceLevel.GetPrice() < order->price){
                break;
            }
            while(order->quantity > 0 && !CurrPriceLevel.IsEmpty()){
                Order* front = CurrPriceLevel.GetHead();
                Quantity MinQuantity = std::min(front->quantity, order->quantity);
                order->quantity -= MinQuantity;
                front->quantity -= MinQuantity;
                if(front->quantity == 0){
                    CurrPriceLevel.RemoveTopOrder();
                }
            }
            if(CurrPriceLevel.IsEmpty()){
                it = bids.erase(it);
            }
            else{
                it++;
            }
        }
        if(order->quantity > 0 && order->type == OrderType::Limit){
            orderbook.AddOrder(order);
        }
    }
}