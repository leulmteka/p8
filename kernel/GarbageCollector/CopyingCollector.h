#ifndef _COPYING_COLLECTOR_H_
#define _COPYING_COLLECTOR_H_

#include "GarbageCollector.h"
#include "stdint.h"
#include "atomic.h"

class CopyingCollector : public GarbageCollector {
private:
    static const uint32_t MARK_BIT = 0x80000000;
    static const uint32_t SIZE_MASK = 0x7FFFFFFF;

    uint32_t* fromLocation;
    uint32_t* toLocation;
    uint32_t sizeOfSpace;
    uint32_t* allocPointer;
    uint32_t* scanPointer;

    void* copying(void* obj);
    void* forward(void* obj);
    bool isForwardAddress(uint32_t* obj);
    uint32_t getForwardAddress(uint32_t* obj);
    void setForwardAddress(uint32_t* obj, uint32_t* newAddress);
    uint32_t getSize(uint32_t* obj);
    void flip();
    void updateReferences();

public:
    CopyingCollector(void* heapStart, size_t heapBytes);
    ~CopyingCollector();

    void* allocate(size_t size) override;
    void free(void* ptr) override;
    void beginCollection() override;
    void garbageCollect() override;
    void endCollection() override;
};

#endif



/*

    Documentation:

    This the header file for CopyingCollector that outlines all the methods we have to implement in order to fully implement copy collecting.

    This algorithm splits the heap into two pieces: the "from" space and the "to" space. New objects are allocated in the "from" space using the
    allocPointer. When the "from" space becomes full, a garbage collection cycle is triggered. In the garbage collection cycle, live objects are copied
    from the "from" space to the "to" space (using copying()). Forwarding addresses are used to update references to the copied objects. 
    The scanPointer is used to track the progress of scanning objects in the "to" space. Scan method scans the objects in the "to" space and updates 
    references to the copied objects. After all live objects are copied, the roles of the "from" and "to" spaces are flipped using the flip function.

    fromLocation is the pointer that points to the start of the from location of the heap

    toLocation is the pointer that points to the start of the to location of the heap

    sizeOfSpace is the size of each space which should be equal to half of the heap

    allocPointer is the pointer used for allocation in the "to" location while in the copying phase. the scanPointer tracks the progress of scanning 
    objects in the "to" space.

    copying() copies the object from the from location to the to location

    forward() forwards the object to its new location in the to location.

    isForwardAddress() this method checks if the object metadata contains a forwarding address

    getForwardAddress() this method gets the the forwarding address from an object metadata.

    setForwardAddress() this method sets the forwarding address of an object in the object's metadata

    getSize() returns the size of the object

    flip() method switches the roles of the from and to locations after a collection cycle

    updateReferences() method scans objects in the to location and updates references to copied objects.
*/