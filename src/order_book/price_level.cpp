#include <iostream>
#include "price_level.hpp"


void PriceLevel::PrintPriceLevel(){
    for(auto &order : orders){
        order->PrintOrder();
    }
}