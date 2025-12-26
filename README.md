# Order Book Project

This project implements a high-performance order book system using modern C++ constructs and efficient coding practices. The order book is designed to handle buy and sell orders, match them efficiently, and provide a robust interface for users and other systems.

## Project Structure

```
order-book
├── src
│   ├── main.cpp                  # Entry point of the application
│   ├── order_book
│   │   ├── order_book.cpp        # Implementation of the OrderBook class
│   │   ├── order_book.hpp        # Declaration of the OrderBook class
│   │   ├── order.cpp             # Implementation of the Order class
│   │   ├── order.hpp             # Declaration of the Order class
│   │   ├── price_level.cpp       # Implementation of the PriceLevel class
│   │   └── price_level.hpp       # Declaration of the PriceLevel class
│   ├── matching_engine
│   │   ├── matching_engine.cpp    # Implementation of the MatchingEngine class
│   │   └── matching_engine.hpp    # Declaration of the MatchingEngine class
│   └── utils
│       ├── memory_pool.hpp       # Memory pool for efficient memory management
│       └── types.hpp             # Common types and constants
├── include
│   └── order_book
│       └── order_book.hpp        # Public interface for the OrderBook class
├── tests
│   ├── test_order_book.cpp       # Unit tests for the OrderBook class
│   ├── test_matching_engine.cpp   # Unit tests for the MatchingEngine class
│   └── test_order.cpp            # Unit tests for the Order class
├── benchmarks
│   └── benchmark_order_book.cpp   # Benchmarking code for performance measurement
├── CMakeLists.txt                # CMake configuration file
└── README.md                     # Project documentation
```

## Setup Instructions

1. **Clone the repository:**
   ```
   git clone <repository-url>
   cd order-book
   ```

2. **Build the project:**
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the application:**
   ```
   ./order-book
   ```

## Usage

The order book system allows users to add, remove, and match orders. The main application will handle user input or data feeds to interact with the order book and matching engine.

## Overview

The order book consists of several key components:

- **OrderBook**: Manages the collection of orders and facilitates order matching.
- **Order**: Represents individual buy or sell orders with properties like order ID, price, and quantity.
- **PriceLevel**: Organizes orders at a specific price point for efficient matching.
- **MatchingEngine**: Processes incoming orders and matches them against existing orders in the order book.
- **MemoryPool**: Provides efficient memory management for orders and price levels.

This project aims to provide a scalable and efficient solution for order management in trading systems.