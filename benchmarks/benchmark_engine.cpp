#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <chrono>
#include "../src/matching_engine/matching_engine.hpp"
#include "../src/utils/rdtsc.hpp"

static constexpr Price BASE_PRICE = 10;
static constexpr int MAX_ORDERS = 100000;

static void PopulateBook(MatchingEngine& engine, OrderID& nextID, int numLevels, int ordersPerLevel){
    Price mid = 5000;
    for(int lvl = 0; lvl < numLevels; lvl++){
        for(int j = 0; j < ordersPerLevel; j++){
            engine.ProcessOrder(nextID % MAX_ORDERS, 100, Side::Sell, OrderType::Limit, mid + 1 + lvl);
            nextID++;
            engine.ProcessOrder(nextID % MAX_ORDERS, 100, Side::Buy,  OrderType::Limit, mid - lvl);
            nextID++;
        }
    }
}

static void BM_AddPassiveOrder(benchmark::State& state){
    MatchingEngine engine(BASE_PRICE);
    OrderID nextID = 0;
    PopulateBook(engine, nextID, 20, 10);

    OrderID benchID = nextID;
    for(auto _ : state){
        OrderID id = benchID % MAX_ORDERS;
        engine.ProcessOrder(id, 100, Side::Buy, OrderType::Limit, 4950);
        engine.CancelOrder(id);
        benchID++;
    }
}
BENCHMARK(BM_AddPassiveOrder);

static void BM_MatchSingleOrder(benchmark::State& state){
    MatchingEngine engine(BASE_PRICE);
    OrderID nextID = 0;
    PopulateBook(engine, nextID, 20, 10);

    for(auto _ : state){
        OrderID sellID = nextID % MAX_ORDERS;
        engine.ProcessOrder(sellID, 100, Side::Sell, OrderType::Limit, 5001);
        nextID++;

        OrderID buyID = nextID % MAX_ORDERS;
        engine.ProcessOrder(buyID, 100, Side::Buy, OrderType::Limit, 5001);
        nextID++;
    }
}
BENCHMARK(BM_MatchSingleOrder);

static void BM_CancelOrder(benchmark::State& state){
    MatchingEngine engine(BASE_PRICE);
    OrderID nextID = 0;
    PopulateBook(engine, nextID, 50, 20);

    int idx = 0;
    for(auto _ : state){
        OrderID target = idx % (50 * 20 * 2);
        engine.CancelOrder(target);
        
        Price price = (target % 2 == 0) ? 5001 + (target / 40) % 50 : 5000 - (target / 40) % 50;
        Side side = (target % 2 == 0) ? Side::Sell : Side::Buy;
        engine.ProcessOrder(target, 100, side, OrderType::Limit, price);
        idx++;
    }
}
BENCHMARK(BM_CancelOrder);

static void BM_SweepFiveLevels(benchmark::State& state){
    MatchingEngine engine(BASE_PRICE);
    OrderID nextID = 0;

    for(auto _ : state){
        for(int i = 0; i < 5; i++){
            engine.ProcessOrder(nextID % MAX_ORDERS, 100, Side::Sell, OrderType::Limit, 5001 + i);
            nextID++;
        }
        engine.ProcessOrder(nextID % MAX_ORDERS, 500, Side::Buy, OrderType::Limit, 5005);
        nextID++;
    }
}
BENCHMARK(BM_SweepFiveLevels);

static void RunLatencyHistogram(){
    MatchingEngine engine(BASE_PRICE);
    constexpr int WARMUP  = 5000;
    constexpr int SAMPLES = 100000;

    std::vector<uint64_t> latencies(SAMPLES);
    OrderID nextID = 0;

    PopulateBook(engine, nextID, 20, 10);

    for(int i = 0; i < WARMUP; i++){
        OrderID sid = nextID % MAX_ORDERS;
        engine.ProcessOrder(sid, 100, Side::Sell, OrderType::Limit, 5001);
        nextID++;
        OrderID bid = nextID % MAX_ORDERS;
        engine.ProcessOrder(bid, 100, Side::Buy, OrderType::Limit, 5001);
        nextID++;
    }

    for(int i = 0; i < SAMPLES; i++){
        OrderID sellID = nextID % MAX_ORDERS;
        engine.ProcessOrder(sellID, 100, Side::Sell, OrderType::Limit, 5001);
        nextID++;

        OrderID buyID = nextID % MAX_ORDERS;

        asm volatile("lfence");
        uint64_t start = rdtsc();

        engine.ProcessOrder(buyID, 100, Side::Buy, OrderType::Limit, 5001);

        uint64_t end = rdtscp();
        latencies[i] = end - start;
        nextID++;
    }

    std::sort(latencies.begin(), latencies.end());
    double ghz = 1.7; // Ultra 5 225H base clock

    printf("\n");
    printf("============================================================\n");
    printf("  rdtsc Latency Histogram: ProcessOrder (match @ best ask)\n");
    printf("============================================================\n");
    printf("  p50     = %6lu cycles  (%6.1f ns)\n", latencies[SAMPLES * 50 / 100],   latencies[SAMPLES * 50 / 100]   / ghz);
    printf("  p90     = %6lu cycles  (%6.1f ns)\n", latencies[SAMPLES * 90 / 100],   latencies[SAMPLES * 90 / 100]   / ghz);
    printf("  p99     = %6lu cycles  (%6.1f ns)\n", latencies[SAMPLES * 99 / 100],   latencies[SAMPLES * 99 / 100]   / ghz);
    printf("  p99.9   = %6lu cycles  (%6.1f ns)\n", latencies[SAMPLES * 999 / 1000], latencies[SAMPLES * 999 / 1000] / ghz);
    printf("============================================================\n\n");
}

static void RunThroughputTest(){
    MatchingEngine engine(BASE_PRICE);
    constexpr int TOTAL_OPS = 500000;
    OrderID nextID = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < TOTAL_OPS; i++){
        OrderID id = nextID % MAX_ORDERS;
        if(i % 2 == 0){
            engine.ProcessOrder(id, 100, Side::Sell, OrderType::Limit, 5001 + (i % 20));
        } 
        else{
            engine.ProcessOrder(id, 100, Side::Buy,  OrderType::Limit, 5001 + ((i - 1) % 20));
        }
        nextID++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();

    printf("============================================================\n");
    printf("  Throughput Test: %d orders (matched pairs)\n", TOTAL_OPS);
    printf("============================================================\n");
    printf("  Elapsed     : %.3f seconds\n", seconds);
    printf("  Throughput  : %.2f million orders/sec\n", TOTAL_OPS / seconds / 1e6);
    printf("  Avg latency : %.1f ns/order\n", seconds / TOTAL_OPS * 1e9);
    printf("============================================================\n\n");
}

int main(int argc, char** argv){
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    RunLatencyHistogram();
    RunThroughputTest();

    return 0;
}
