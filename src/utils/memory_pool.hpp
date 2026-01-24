#include <cstddef>
#include <vector>

template <typename T, size_t BlockSize = 4096>
class MemoryPool{
    public:

        union Block{
            T element;
            Block* next;
        };

    MemoryPool() {}

    ~MemoryPool() {
        for(auto &blocks : chunks){
            delete [] blocks;
        }
    }

    

    T* allocate(){
        if(freeHead != nullptr){
            Block* allocatedBlock = freeHead;
            freeHead = freeHead->next;
            return &(allocatedBlock->element);
        }
        else{
            expandPool();
            return allocate();
        }
    }

    void deallocate(T* ptr){
        Block* blockFree = reinterpret_cast<Block*>(ptr);
        blockFree->next = freeHead;
        freeHead = blockFree;
    }

    private :
        Block* freeHead = nullptr;
        std::vector<Block*> chunks;

        void expandPool(){ //in case the existing pool is full
            Block* newChunk = new Block[BlockSize];
            for(int i = 0; i < BlockSize - 1; i++){
                newChunk[i].next = &(newChunk[i + 1]);
            }
            newChunk[BlockSize - 1].next = nullptr;
            chunks.push_back(newChunk);
            freeHead = newChunk;
        }
};  