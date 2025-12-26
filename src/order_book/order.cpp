#include <iostream>
#include "order.hpp"


void Order::PrintOrder(){
    std::cout << "OrderID - " << ID << "\n";
    std::cout << "Current Quantity - " << quantity << "\n";
    std::cout << "Initial Quantity - " << init_quantity << "\n";
    std::cout << "Order Type  - ";
    if(type == OrderType::Limit){
        std::cout << "Limit" << "\n";
    }
    else if(type == OrderType::Market){
        std::cout << "Market" << "\n";
    }
    else if(type == OrderType::FOK){
        std::cout << "FOK" << "\n";
    }
    else if(type == OrderType::IOC){
        std::cout << "IOC" << "\n";
    }
}
