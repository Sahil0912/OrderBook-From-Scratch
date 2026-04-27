#include "price_level.hpp"


void PriceLevel::PrintPriceLevel(){
    auto ptr = head;
    while(ptr != nullptr){
        ptr->PrintOrder();
        ptr = ptr->next;
    }
}