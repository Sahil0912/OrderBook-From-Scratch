#include "matching_engine.hpp"
#include <algorithm>


MatchingEngine::MatchingEngine(Price basePrice) : book_(basePrice) {}

void MatchingEngine::ProcessOrder(Order* order){
    lookup_.Insert(order);
    if(order->side == Side::Buy){
        Price askPrice = book_.GetBestAskPrice();
        Price maxAsk = book_.GetBasePrice() + MAX_PRICE_LEVELS - 1;
        while(order->quantity > 0 && askPrice <= maxAsk){
            
            if(order->type == OrderType::Limit && order->price < askPrice) break;

            auto& currLevel = book_.GetLevel(askPrice);
            while(order->quantity > 0 && !currLevel.IsEmpty()){
                Order* front = currLevel.GetHead();
                Quantity MinQuantity = std::min(front->quantity, order->quantity);
                order->quantity -= MinQuantity;
                front->quantity -= MinQuantity;
                if(front->quantity == 0){
                    currLevel.RemoveTopOrder();
                    lookup_.Remove(front->ID);
                    pool_.deallocate(front);
                }
            }
            if(currLevel.IsEmpty()){
                //finding next non-empty ask in O(N/64)
                askPrice = book_.FindNextAsk(askPrice + 1);
                book_.SetBestAskPrice(askPrice);
            }
        }
        if(order->quantity > 0 && order->type == OrderType::Limit){
            book_.AddOrder(order);
        }
        else{
            lookup_.Remove(order->ID);
            pool_.deallocate(order);
        }

    }
    else{
        Price bidPrice = book_.GetBestBidPrice();
        Price minBid = book_.GetBasePrice();
        while(order->quantity > 0 && bidPrice >= minBid && bidPrice != 0){
            
            if(order->type == OrderType::Limit && order->price > bidPrice) break;

            auto& currLevel = book_.GetLevel(bidPrice);
            while(order->quantity > 0 && !currLevel.IsEmpty()){
                Order* front = currLevel.GetHead();
                Quantity MinQuantity = std::min(front->quantity, order->quantity);
                order->quantity -= MinQuantity;
                front->quantity -= MinQuantity;
                if(front->quantity == 0){
                    currLevel.RemoveTopOrder();
                    lookup_.Remove(front->ID);
                    pool_.deallocate(front);
                }
            }
            if(currLevel.IsEmpty()){
                if(bidPrice > minBid){
                    bidPrice = book_.FindNextBid(bidPrice - 1);
                }
                else{
                    bidPrice = 0;
                }
                book_.SetBestBidPrice(bidPrice);
            }
        }
        if(order->quantity > 0 && order->type == OrderType::Limit){
            book_.AddOrder(order);
        }
        else{
            lookup_.Remove(order->ID);
            pool_.deallocate(order);
        }
    }
}

void MatchingEngine::ProcessOrder(OrderID id, Quantity qty, Side side, OrderType type, Price price){
    Order* order = pool_.emplace(id, qty, side, type, price);
    if(!order) return; //pool exhausted
    MatchingEngine::ProcessOrder(order);
}

bool MatchingEngine::CancelOrder(OrderID id){
    Order* order = lookup_.Find(id);
    if(!order) return false;
    book_.RemoveOrder(order);
    lookup_.Remove(id);
    pool_.deallocate(order);
    return true;
}
bool MatchingEngine::CancelOrder(Order* order){
    if(!order) return false;
    book_.RemoveOrder(order);
    lookup_.Remove(order->ID);
    pool_.deallocate(order);
    return true;
}
void MatchingEngine::PrintOrderBook() const{
    book_.PrintOrderBook();
}