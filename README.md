# NanoMatch — Nanosecond Matching Engine

A low-latency limit order book and matching engine written in C++17, designed from the ground up with cache-friendly, zero-allocation data structures. No STL containers on the hot path.

## Setup & Running

```bash
git clone https://github.com/Sahil0912/NanoMatch.git
cd NanoMatch
mkdir -p build && cd build
cmake ..
make
./OrderBookEngine
```

Requires: CMake ≥ 3.13, a C++17 compiler (GCC/Clang).

## What I've Built So Far

### Cache-Optimized Memory Management
- **Custom Memory Pool** - Pre-allocates a contiguous block of `Order` slots at startup. Allocation and deallocation are O(1) via an intrusive free-list. Zero `malloc` calls on the hot path.
- **Intrusive Doubly-Linked Lists** - `prev`/`next` pointers live inside the `Order` struct itself. No external container nodes, no `std::deque`, no heap-allocated iterators.

### Flat Array Order Book
- Replaced `std::map<Price, PriceLevel>` (red-black tree with pointer-chasing cache misses) with a **fixed-size contiguous array** indexed by price tick. Price level lookup is O(1) — a single pointer arithmetic operation.
- Manual best-bid/best-ask tracking with linear recalculation over contiguous memory when a level empties.

### O(1) Order Cancellation
- **Flat sparse lookup table** mapping `OrderID → Order*` for O(1) find. No hash maps, no heap allocation.
- Full cancel lifecycle: lookup → DLL unlink → best-price recalc → pool deallocation. All O(1).

### Self-Contained Matching Engine
- `MatchingEngine` owns the order book, memory pool, and lookup table — single point of ownership for the entire order lifecycle.
- Filled passive orders are deallocated back to the pool immediately (no memory leaks).
- Supports Limit and Market order types with strict price-time priority.

## What I am planning to do in the future

- [ ] **Benchmarking Harness** - Google Benchmark integration + `rdtsc` inline wrappers for cycle-accurate latency measurement. Latency percentiles (p50, p90, p99, p99.9) and throughput (orders/sec).
- [ ] **Order Modification** - Cancel-replace with correct price-time priority semantics (keep priority on qty decrease, lose priority on price change).
- [ ] **Lock-Free Trade Logging** - SPSC ring buffer using `std::atomic` for async trade execution reporting without blocking the matching path.
- [ ] **Zero-Copy Data Ingestion** - `mmap`-based market data parser bypassing `std::ifstream` entirely. Read directly from the kernel page cache.