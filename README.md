# NanoMatch - Map-Based Order Book (Legacy Branch)

This branch contains the initial `std::map`-based implementation of the matching engine. It is functionally correct but uses STL containers that cause cache misses and heap allocations on the hot path. See the `flat-orderbook` branch for the optimized version.

## Setup & Running

```bash
git clone https://github.com/Sahil0912/NanoMatch.git
cd NanoMatch
git checkout map-orderbook
mkdir -p build && cd build
cmake ..
make
./OrderBookEngine
```

Requires: CMake ≥ 3.13, a C++17 compiler (GCC/Clang).

## What's in This Branch

### Map-Based Order Book
- Bids stored in `std::map<Price, PriceLevel, std::greater<Price>>` (sorted high → low).
- Asks stored in `std::map<Price, PriceLevel>` (sorted low → high).
- Top-of-book via `begin()` — O(1) amortized but involves pointer dereferences to heap-allocated tree nodes.

### Memory Pool
- Custom `MemoryPool<Order>` with intrusive free-list for O(1) allocation/deallocation.
- Orders are allocated from a contiguous pre-allocated block — no `malloc` on the hot path.

### Matching Engine
- Stateless `MatchingEngine` — receives an `Order*` and `OrderBook&` externally.
- Traverses the map via iterators to match aggressive orders against resting orders.
- Supports Limit and Market order types.

### Intrusive Linked Lists
- Each `PriceLevel` manages orders as an intrusive doubly-linked list (`Order::prev`, `Order::next`).
- No `std::deque` or external container nodes.
