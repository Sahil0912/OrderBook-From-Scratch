#include "matching_engine.hpp"
#include <algorithm>

void MatchingEngine::ProcessOrder(Order* order, OrderBook &orderbook){
    if(order->side == Side::Buy){
        Price askPrice = orderbook.GetBestAskPrice();
        Price maxAsk = orderbook.GetBasePrice() + MAX_PRICE_LEVELS - 1;
        while(order->quantity > 0 && askPrice <= maxAsk){
            
            if(order->type == OrderType::Limit && order->price < askPrice) break;

            auto& currLevel = orderbook.GetLevel(askPrice);
            while(order->quantity > 0 && !currLevel.IsEmpty()){
                Order* front = currLevel.GetHead();
                Quantity MinQuantity = std::min(front->quantity, order->quantity);
                order->quantity -= MinQuantity;
                front->quantity -= MinQuantity;
                if(front->quantity == 0){
                    currLevel.RemoveTopOrder();
                }
            }
            if(currLevel.IsEmpty()){
                askPrice++;
                while(askPrice <= maxAsk && orderbook.GetLevel(askPrice).IsEmpty()){
                    askPrice++;
                }
                orderbook.SetBestAskPrice(askPrice);
            }
        }
        if(order->quantity > 0 && order->type == OrderType::Limit){
            orderbook.AddOrder(order);
        }

    }
    else{
        Price bidPrice = orderbook.GetBestBidPrice();
        Price minBid = orderbook.GetBasePrice();
        while(order->quantity > 0 && bidPrice >= minBid && bidPrice != 0){
            
            if(order->type == OrderType::Limit && order->price > bidPrice) break;

            auto& currLevel = orderbook.GetLevel(bidPrice);
            while(order->quantity > 0 && !currLevel.IsEmpty()){
                Order* front = currLevel.GetHead();
                Quantity MinQuantity = std::min(front->quantity, order->quantity);
                order->quantity -= MinQuantity;
                front->quantity -= MinQuantity;
                if(front->quantity == 0){
                    currLevel.RemoveTopOrder();
                }
            }
            if(currLevel.IsEmpty()){
                bidPrice--;
                while(bidPrice >= minBid && bidPrice != 0 && orderbook.GetLevel(bidPrice).IsEmpty()){
                    bidPrice--;
                }
                orderbook.SetBestBidPrice(bidPrice);
            }
        }
        if(order->quantity > 0 && order->type == OrderType::Limit){
            orderbook.AddOrder(order);
        }
    }
}