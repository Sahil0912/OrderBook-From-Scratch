#pragma once
#include <atomic>
#include <cstddef>
#include <type_traits>

template<typename T, size_t CAPACITY>
class SPSCRing {

    static_assert(CAPACITY > 0, "Capacity shoudl be > 0");
    static_assert((CAPACITY & (CAPACITY - 1)) == 0, "Capacity should be power of 2");
    static_assert(std::is_trivially_copyable_v<T>, "Type should be trivially copyable (no custom copy or move constructors)");

private:
    static constexpr size_t MASK = CAPACITY - 1;

    alignas(64) std::atomic<size_t> head_{0}; //producer writes
    alignas(64) std::atomic<size_t> tail_{0}; //consumer writes
    T buffer_[CAPACITY];

public:
    SPSCRing() = default;

    //delete copy & move
    SPSCRing(const SPSCRing&)            = delete;
    SPSCRing& operator=(const SPSCRing&) = delete;
    SPSCRing(SPSCRing&&)                 = delete;
    SPSCRing& operator=(SPSCRing&&)      = delete;


    //Producer side
    bool push(const T& item) {
        size_t h = head_.load(std::memory_order_relaxed);
        size_t t = tail_.load(std::memory_order_acquire);
        if(h - t == CAPACITY) return false;
        buffer_[h & MASK] = item;
        head_.store(h + 1, std::memory_order_release);
        return true;
    }

    //Consumer side
    bool pop(T& item) {
        size_t t = tail_.load(std::memory_order_relaxed);
        size_t h = head_.load(std::memory_order_acquire);
        
        if(t == h) return false;
        item = buffer_[t & MASK];
        tail_.store(t + 1, std::memory_order_release);
        return true;
    }

    size_t size() const {
        size_t h = head_.load(std::memory_order_relaxed);
        size_t t = tail_.load(std::memory_order_relaxed);
        return h - t;
    }
};
