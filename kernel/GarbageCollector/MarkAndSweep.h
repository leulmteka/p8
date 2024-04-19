#ifndef _MARK_AND_SWEEP_H_
#define _MARK_AND_SWEEP_H_

#include "GarbageCollector.h"
#include "stdint.h"
#include "atomic.h"
#include "blocking_lock.h"

class MarkAndSweep : public GarbageCollector {
private:

    //h
    static const uint32_t MARKED = 0x80000000;
    static const uint32_t GET_SIZE = 0x7FFFFFFF;
    static const size_t MAX_GLOBAL_OBJECTS = 1024;
    static const size_t MAX_STATIC_OBJECTS = 1024;

    uint32_t* heap;
    uint32_t sizeOfHeap;
    static int safe;
    static int avail;
    static BlockingLock *heapLock;
    static bool interruptState;
    static uint32_t** globalObjects;
    static uint32_t** staticObjects;
    static uint32_t numGlobalObjects;
    static uint32_t numStaticObjects;

    bool* marks;

    
    //A Heap Lock? Concurrency? 

    bool isPointer(uint32_t* field) {return true;};
    void sweep() {};
    //uint32_t* allocateFreeMem(uint32_t size);


public:
    MarkAndSweep(void* heapStart, size_t bytes){
        marks = new bool[bytes/sizeof(uint32_t)];
        //marks[bytes/sizeof(uint32_t)];
        // for(uint32_t i = 0; i < bytes/sizeof(uint32_t); i++){
        //     Debug::printf("%d\n", marks[i]);
        // }
    }
    ~MarkAndSweep(){};

    void* allocate(size_t size) override{return nullptr;};
    void free(void* ptr) override{};
    void beginCollection() override{};
    void garbageCollect() override{};
    void endCollection() override{};

    static void addGlobalObject(uint32_t* obj){};
    static void addStaticObject(uint32_t* obj){};

    void markBlock(int blockIndex) {
        // Set the mark for a block
        //if (blockIndex < marks.size()) {
            marks[blockIndex] = true;
        //}
    }
    void unmarkBlock(int blockIndex) {
        // Clear the mark for a block
       
        marks[blockIndex] = false;
    }

};

#endif



/*

    Documentation:
    
    This the header file for MarkAndSweep that outlines all the methods we have to implement in order to fully implement mark and sweep.

    +---------------+
    |    Metadata   |
    +---------------+
    |               |
    |  Object Data  |
    |               |
    +---------------+

    MARKED is 0x80000000; This sets the MSB to 1 and all the others to 0. Using the MARKED variable, we can mark objects during the mark 
    phase. Metadata |= MARKED will mark the object and Metadata & MARK_BIT) != 0 will check if the object is marked

    GET_SIZE is 0x7FFFFFFF; MSB is set to 0 and all the others to 1. This allows us to extract the size of an object from its metadata word,
    ignoring the mark bit. This assumes that the size of the object is in the lower 31 bits. Metadata & GET_SIZE gets the size of the object.

    heap variable is a pointer to the start of hte heap mem

    sizeOfHeap is the size of the heap in bytes

    markObject() marks the object

    sweep() iterates over the heap and reclaims unmarked objects

    allocateFreeMem() allocates memory from the free list

    isMarked() checks if the object is marked

*/