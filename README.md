# NanoMatch — Nanosecond Matching Engine

A low-latency limit order book and matching engine written in C++17, designed from the ground up with cache-friendly, zero-allocation data structures. No STL containers on the hot path.

## Setup & Running

```bash
git clone https://github.com/Sahil0912/NanoMatch.git
cd NanoMatch
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

To run the basic engine:
```bash
./OrderBookEngine
```

To run the performance suite (Google Benchmark):
```bash
./bench_engine
```

Requires: CMake ≥ 3.13, a C++17 compiler (GCC/Clang).

## What I've Built So Far

### Cache-Optimized Memory Management
- **Custom Memory Pool** - Pre-allocates a contiguous block of `Order` slots at startup. Allocation and deallocation are O(1) via an intrusive free-list. Zero `new`/`malloc` calls on the hot path.
- **Intrusive Doubly-Linked Lists** - `prev`/`next` pointers live inside the `Order` struct itself. No external container nodes, no `std::deque`, no heap-allocated iterators.

### Flat Array Order Book (with Bitmap Acceleration)
- Replaced `std::map<Price, PriceLevel>` (red-black tree with pointer-chasing cache misses) with a **fixed-size contiguous array** indexed directly by price tick. Price level lookup is strictly O(1).
- **Algorithmic Bitmap Acceleration** - Eliminated catastrophic O(N) scanning latency by wrapping the array in an L1-resident 12KB bitmap. Leverages CPU bit-manipulation intrinsics (`tzcnt`/`lzcnt` via `__builtin_ctzll`) to jump directly to the next active price level in a single CPU cycle (`O(N/64)`).

### O(1) Order Cancellation
- **Flat sparse lookup table** mapping `OrderID → Order*` for O(1) find. No hash maps, no heap allocation.
- Full cancel lifecycle: lookup → DLL unlink → best-price bitmap recalc → pool deallocation. All O(1).

### Self-Contained Matching Engine
- `MatchingEngine` owns the order book, memory pool, and lookup table — single point of ownership for the entire order lifecycle.
- Filled passive orders are deallocated back to the pool immediately (no memory leaks).
- Supports Limit and Market order types with strict price-time priority.

## Performance Profiling & Benchmarking

Compiled with `-O3 -march=native` and rigorously tested using `asm volatile` assembly memory clobbers to prevent compiler Dead Code Elimination (DCE).

- **Throughput:** 28+ million orders/sec
- **Matching Latency (p50):** ~85 ns 
- **Tail Latency (p99.9):** Deterministic sub-150ns execution
- **Sweep Optimization:** Empty-level scans are ~250x faster due to bitmap hardware intrinsics.
- Built custom cycle-accurate `rdtsc` and `rdtscp` fences to measure absolute cycle latency directly from the CPU, bypassing slow OS clock boundaries (`std::chrono`).

## What I am planning to do in the future

- [ ] **Order Modification** - Cancel-replace with correct price-time priority semantics (keep priority on qty decrease, lose priority on price change).
- [ ] **Lock-Free Trade Logging** - SPSC ring buffer using `std::atomic` for async trade execution reporting without blocking the matching path.
- [ ] **Zero-Copy Data Ingestion** - `mmap`-based market data parser bypassing `std::ifstream` entirely. Read directly from the kernel page cache.