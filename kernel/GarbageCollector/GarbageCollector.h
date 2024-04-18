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
};

#endif