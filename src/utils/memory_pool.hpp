#pragma once

#include <cstddef>
#include <utility>

template <typename T, size_t CAPACITY> //capacity is toatal number of slots in the pool
class MemoryPool{
    public:

        union Block{
            T element; //slot is alive
            Block* next; //slot is free

            Block(){}
            ~Block(){}
        };

    MemoryPool() {
        chunk_ = new Block[CAPACITY]; 
        freeHead = &chunk_[0];
        count_ = 0;
        for(size_t i = 0; i < CAPACITY - 1; i++){
            chunk_[i].next = &chunk_[i + 1];
        }
        chunk_[CAPACITY - 1].next = nullptr;
    }

    ~MemoryPool() {
        delete[] chunk_;
    }

    //Deleting copying and moving operations
    MemoryPool(const MemoryPool&)            = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&)                 = delete;
    MemoryPool& operator=(MemoryPool&&)      = delete;

    

    T* allocate(){ //Just for the allocation (we have to contruct the T inplace of this alloted memory)
        if(!freeHead){ //pool exhausted
            return nullptr; // I will be discarding the orders when the pool is exhausted
        }
        else{
            count_++;
            Block* slot = freeHead;
            freeHead = freeHead->next;
            return reinterpret_cast<T*>(slot);
        }
    }
    template<typename... Args>
    T* emplace(Args&&... args){
        T* slot = allocate();
        if(!slot) return nullptr;

        return new(slot) T(std::forward<Args>(args)...);
    }

    void deallocate(T* ptr){
        if(!ptr) return; //error handling
        ptr->~T();
        Block* blockFree = reinterpret_cast<Block*>(ptr);
        blockFree->next = freeHead;
        freeHead = blockFree;
        --count_;
    }

    size_t size() const { return count_; }
    size_t capacity() const { return CAPACITY; }
    bool full() const { return count_ == CAPACITY; }

    private :
        Block* freeHead = nullptr;
        Block* chunk_ = nullptr;
        size_t count_; // number of alive blocks (allocated)
};