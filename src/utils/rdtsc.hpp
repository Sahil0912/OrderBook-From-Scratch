#pragma once
#include <cstdint>

//fenced rdtsc- use before the measured section.
//lfence ensures all prior instructions complete before reading the counter.
static inline uint64_t rdtsc(){
    uint32_t lo, hi;
    asm volatile(
        "lfence\n\t"
        "rdtsc"
        : "=a"(lo), "=d"(hi)
    );
    return ((uint64_t)hi << 32) | lo;
}

//serializing rdtscp- use after the measured section.
//waits for all prior instructions (including the measured code) to complete.
static inline uint64_t rdtscp(){
    uint32_t lo, hi, aux;
    asm volatile(
        "rdtscp"
        : "=a"(lo), "=d"(hi), "=c"(aux)
    );
    return ((uint64_t)hi << 32) | lo;
}
