#include "matching_engine.hpp"

void MatchingEngine::ProcessOrder(std::shared_ptr<Order> order, OrderBook &orderbook){
    // so first I have to figure out what to match it with right ?
    if(order->side == Side::Buy){
        // now if its on buy side I have to check in the ask side of it before merging it in bid
        const auto& asks = orderbook.GetAsks();
        auto it = asks.begin();
        while(order->quantity > 0 && it != asks.end()){
            auto CurrPriceLevel = it->second;
            if(order->type == OrderType::Limit && (CurrPriceLevel->GetPrice()) > order->price){
                break;
            }
            // it-> second has the PriceLevel
            // So i want to get as many order as possible 
            const auto& OrdersList = CurrPriceLevel->GetOrders();
            while(order->quantity > 0 && !CurrPriceLevel->IsEmpty()){
                Quantity MinQuantity = std::min((OrdersList.front())->quantity, order->quantity);
                order->quantity -= MinQuantity;
                OrdersList.front()->quantity -= MinQuantity;
                if(OrdersList.front()->quantity == 0){
                    CurrPriceLevel->RemoveTopOrder();
                }
            }
            if(CurrPriceLevel->IsEmpty()){
                it++;
                orderbook.RemoveAskLevel(CurrPriceLevel->GetPrice());     
            }
        }
        if(order->quantity > 0 && order->type == OrderType::Limit){
            orderbook.AddOrder(order);
        }
    }
    else{

        const auto& bids = orderbook.GetBids();

        auto it = bids.begin();

        while(order->quantity > 0 && it != bids.end()){
            auto CurrPriceLevel = it->second;
            if(order->type == OrderType::Limit && (CurrPriceLevel->GetPrice()) < order->price){
                break;
            }
            const auto& OrdersList = CurrPriceLevel->GetOrders();
            while(order->quantity > 0 && !CurrPriceLevel->IsEmpty()){
                Quantity MinQuantity = std::min((OrdersList.front())->quantity, order->quantity);
                order->quantity -= MinQuantity;
                OrdersList.front()->quantity -= MinQuantity;
                if(OrdersList.front()->quantity == 0){
                    CurrPriceLevel->RemoveTopOrder();
                }
            }
            if(CurrPriceLevel->IsEmpty()){
                it++;
                orderbook.RemoveBidLevel(CurrPriceLevel->GetPrice());     
            }
        }
        if(order->quantity > 0 && order->type == OrderType::Limit){
            orderbook.AddOrder(order);
        }

    }
}