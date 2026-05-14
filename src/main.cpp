#include "matching_engine/matching_engine.hpp"

#include <thread>
#include <cstdio>


std::atomic<bool> running{true};

void loggerThread(SPSCRing<TradeEvent, 65536>& ring) {
    FILE* logFile = fopen("trades.log", "w");
    TradeEvent evt;
    
    while (running.load(std::memory_order_relaxed)) {
        if (ring.pop(evt)) {
            fprintf(logFile, "Aggressor ID : %lu\nResting ID : %lu\nPrice : %lu\nQuantity : %lu\nAggressor Side : %u\nTimeStamp : %lu\n\n",
                    evt._aggressorID, evt._restingID,
                    evt._price, evt._quantity,
                    (unsigned)evt._aggressorSide, evt.timestamp);
        }
    }
    // drain remaining events after shutdown
    while (ring.pop(evt)) {
        fprintf(logFile, "Aggressor ID : %lu\nResting ID : %lu\nPrice : %lu\nQuantity : %lu\nAggressor Side : %u\nTimeStamp : %lu\n\n",
                evt._aggressorID, evt._restingID,
                evt._price, evt._quantity,
                (unsigned)evt._aggressorSide, evt.timestamp);
    }
    fclose(logFile);
}


int main(){
    MatchingEngine engine(10);
    
    std::thread logger(loggerThread, std::ref(engine.getTradeRing()));

    engine.ProcessOrder(1, 100, Side::Sell, OrderType::Limit, 100);
    engine.ProcessOrder(2, 50,  Side::Sell, OrderType::Limit, 101);
    engine.ProcessOrder(3, 200, Side::Buy,  OrderType::Limit, 102);

    running.store(false, std::memory_order_relaxed);
    logger.join();
    
    engine.PrintOrderBook();

    printf("Dropped events: %lu\n", engine.getDroppedEvents());

    return 0;
}
