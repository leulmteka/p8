#ifndef _MARK_AND_SWEEP_H_
#define _MARK_AND_SWEEP_H_

#include "GarbageCollector.h"
#include "stdint.h"
#include "atomic.h"
#include "blocking_lock.h"

class MarkAndSweep : public GarbageCollector {
private:
    static const uint32_t MARKED = 0x80000000;
    static const uint32_t GET_SIZE = 0x7FFFFFFF;

    uint32_t* heap;
    uint32_t sizeOfHeap;
    static int safe;
    static int avail;
    static BlockingLock *heapLock;
    
    //A Heap Lock? Concurrency? 

    void markObject(uint32_t* obj);
    void sweep();
    //uint32_t* allocateFreeMem(uint32_t size);


public:
    MarkAndSweep(void* heapStart, size_t bytes);
    ~MarkAndSweep();

    void* allocate(size_t size) override;
    void free(void* ptr) override;
    void beginCollection() override;
    void garbageCollect() override;
    void endCollection() override;
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