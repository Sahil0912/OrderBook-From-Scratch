#include "matching_engine/matching_engine.hpp"

int main(){
    MatchingEngine engine(10);
    
    engine.ProcessOrder(1, 100, Side::Sell, OrderType::Limit, 100);
    engine.ProcessOrder(2, 50,  Side::Sell, OrderType::Limit, 101);
    engine.ProcessOrder(3, 200, Side::Buy,  OrderType::Limit, 99);
    
    engine.PrintOrderBook();
    
    engine.CancelOrder(1);
    engine.PrintOrderBook();
    
    return 0;
}
