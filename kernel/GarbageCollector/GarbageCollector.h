#ifndef _GARBAGE_COLLECTOR_H_
#define _GARBAGE_COLLECTOR_H_

#include "stdint.h"
#include "atomic.h"

class GarbageCollector {
public:
    virtual ~GarbageCollector() {}

    virtual void* allocate(size_t size) = 0;

    virtual void free(void* ptr) = 0;

    virtual void beginCollection() = 0;

    virtual void garbageCollect() = 0;

    virtual void endCollection() = 0;

    //We can also use methods from heap.cc such as new and delete here.
};

#endif



/*
    Documentation:

    This file is meant as an interface so that MarkAndSweep, MarkAndCompact, and CopyingCollector can common implementations.

    allocate(size) allocates memory for the object of size and returns a pointer to the allocated mem

    free(ptr) frees the memory occupied by the object pointed to by ptr.

    beginCollection() method starts the garbage collection cycle. Does setup as well

    garbageCollect() method does the actual garbage collection with different algorithms based on type of GC

    endCollection() ends the GC cycle. Does any cleanup as well.

*/