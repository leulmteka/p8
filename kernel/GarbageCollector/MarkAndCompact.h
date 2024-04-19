#ifndef _MARK_AND_COMPACT_H_
#define _MARK_AND_COMPACT_H_

#include "GarbageCollector.h"
#include "stdint.h"
#include "debug.h"
#include "blocking_lock.h"
#include "semaphore.h"
#include "atomic.h"
#include "shared.h"
#include "threads.h"
#include "config.h"
#include "process.h"


class MarkAndCompact : public GarbageCollector
{
private:
    static const uint32_t MARK_BIT = 0x80000000;
    static const uint32_t SIZE_MASK = 0x7FFFFFFF;
    static const size_t MAX_GLOBAL_OBJECTS = 1024;
    static const size_t MAX_STATIC_OBJECTS = 1024;

    uint32_t *heap;
    uint32_t sizeOfHeap;
    uint32_t *compactPointer;
    static int safe;
    static int avail;
    static BlockingLock *heapLock;
    static uint32_t **globalObjects;
    static uint32_t **staticObjects;
    static uint32_t numGlobalObjects;
    static uint32_t numStaticObjects;

    void markObject(uint32_t *obj);
    void compact();
    void update();
    // uint32_t* allocateFromHeap(uint32_t size); would it just be the same as allocate method below?
    bool isMarked(uint32_t *obj);

public:
    MarkAndCompact(void *heapStart, size_t heapBytes);
    ~MarkAndCompact();

    void *allocate(size_t size) override;
    void free(void *ptr) override;
    void beginCollection() override;
    void garbageCollect() override;
    void endCollection() override;

    static void addGlobalObject(uint32_t *obj);
    static void addStaticObject(uint32_t *obj);
};

#endif

/*

    Documentation:

    This the header file for MarkAndCompact that outlines all the methods we have to implement in order to fully implement mark and compact.

    During marking phase, the markObject function is used to mark all reachable objects by setting the mark bit in their metadata. In the compact phase,
    the compact function relocates live objects towards the beginning of the heap, reducing fragmentation. The compactPointer tracks the destination
    location for each live object. The update function is then called to update references to objects after compaction so they point to the new locations.

    MARKED is 0x80000000; This sets the MSB to 1 and all the others to 0. Using the MARKED variable, we can mark objects during the mark
    phase. Metadata |= MARKED will mark the object and Metadata & MARK_BIT) != 0 will check if the object is marked

    GET_SIZE is 0x7FFFFFFF; MSB is set to 0 and all the others to 1. This allows us to extract the size of an object from its metadata word,
    ignoring the mark bit. This assumes that the size of the object is in the lower 31 bits. Metadata & GET_SIZE gets the size of the object

    heap variable is a pointer to the start of hte heap mem

    sizeOfHeap is the size of the heap in bytes

    compactPointer is the pointer that we will be using during the compact phase to keep track of the destination for live objects.

    markObject() marks the object

    compact() compact phase: relocates live objects towards the beginning of the heap

    update() updates references to objects after compact to ensure they point to the new locations

    isMarked() checks if the object is marked

*/