#include "heap.h"
#include "debug.h"
#include "stdint.h"
#include "blocking_lock.h"
#include "atomic.h"
#include "GarbageCollector/MarkAndSweep.h"
/* A first-fit heap */


namespace gheith {
    
int *array; //a "free" list
int len;
int safe = 0;
static int avail = 0; // head of free list
static BlockingLock *theLock = nullptr;
MarkAndSweep* GC = nullptr;

void makeTaken(int i, int ints);
void makeAvail(int i, int ints);

int abs(int x) {
    if (x < 0) return -x; else return x;
}

int size(int i) {
    return abs(array[i]);
}

int headerFromFooter(int i) {
    return i - size(i) + 1;
}

int footerFromHeader(int i) {
    return i + size(i) - 1;
}
    
int sanity(int i) {
    if (safe) {
        if (i == 0) return 0;
        if ((i < 0) || (i >= len)) {
            Debug::panic("bad header index %d\n",i);
            return i;
        }
        int footer = footerFromHeader(i);
        if ((footer < 0) || (footer >= len)) {
            Debug::panic("bad footer index %d\n",footer);
            return i;
        }
        int hv = array[i];
        int fv = array[footer];
  
        if (hv != fv) {
            Debug::panic("bad block at %d, hv:%d fv:%d\n", i,hv,fv);
            return i;
        }
    }

    return i;
}

int left(int i) {
    return sanity(headerFromFooter(i-1));
}

int right(int i) {
    return sanity(i + size(i));
}

int next(int i) {
    return sanity(array[i+1]);
}

int prev(int i) {
    return sanity(array[i+2]);
}

void setNext(int i, int x) {
    array[i+1] = x;
}

void setPrev(int i, int x) {
    array[i+2] = x;
}

void remove(int i) {
    int prevIndex = prev(i);
    int nextIndex = next(i);

    if (prevIndex == 0) {
        /* at head */
        avail = nextIndex;
    } else {
        /* in the middle */
        setNext(prevIndex,nextIndex);
    }
    if (nextIndex != 0) {
        setPrev(nextIndex,prevIndex);
    }
}

void makeAvail(int i, int ints) {
    array[i] = ints;
    array[footerFromHeader(i)] = ints;    
    setNext(i,avail);
    setPrev(i,0);
    if (avail != 0) {
        setPrev(avail,i);
    }
    avail = i;
}

void makeTaken(int i, int ints) {
    array[i] = -ints;
    array[footerFromHeader(i)] = -ints;    
}

int isAvail(int i) {
    return array[i] > 0;
}

int isTaken(int i) {
    return array[i] < 0;
}
void printHeap()
{
    int p = 0;
    while (p < len)
    {
        int blockSize = abs(size(p)); // Get the absolute size to print regardless of block status
        if (isAvail(p)) {
            int headerValue = array[p];                   // This is the header value which contains size and status (positive if available)
            //int footerValue = array[footerFromHeader(p)]; // Footer value for consistency check
            Debug::printf("| Free Block at %d: header = %d, size = %d, footer = %d\n",
                p, headerValue, blockSize, footerFromHeader(p));
        }
        p += blockSize; // Move to the next block
    }
}
};

void heapInit(void* base, size_t bytes) {
    using namespace gheith;

    Debug::printf("| heap range 0x%x 0x%x\n",(uint32_t)base,(uint32_t)base+bytes);

    /* can't say new becasue we're initializing the heap */
    array = (int*) base;
    len = bytes / 4;
    makeTaken(0,2);
    makeAvail(2,len-4);
    makeTaken(len-2,2);
    theLock = new BlockingLock();
    GC = new MarkAndSweep(base, bytes); //for now    
    Debug::printf("%x i\n", bytes);
}

void* malloc(size_t bytes) {
    using namespace gheith;
    //Debug::printf("malloc(%d)\n",bytes);
    if (bytes == 0) return (void*) array;   

    int ints = ((bytes + 3) / 4) + 2;
    if (ints < 4) ints = 4;

    LockGuardP g{theLock};

    void* res = 0;

    int mx = 0x7FFFFFFF;
    int it = 0;

    {
        int countDown = 20;
        int p = avail;
        while (p != 0) {
            if (!isAvail(p)) {
                Debug::panic("block is not available in malloc %p\n",p);
            }
            int sz = size(p);

            if (sz >= ints) {
                if (sz < mx) {
                    mx = sz;
                    it = p;
                }
                countDown --;
                if (countDown == 0) break;
            }
            p = next(p);
        }
    }

    if (it != 0) {
        remove(it);
        int extra = mx - ints;
        if (extra >= 4) {
            makeTaken(it,ints);
            makeAvail(it+ints,extra);
        } else {
            makeTaken(it,mx);
        }
        res = &array[it+1];
    }

    return res;
}

void free(void* p) {
    using namespace gheith;
    if (p == 0) return;
    if (p == (void*) array) return;

    LockGuardP g{theLock};

    int idx = ((((uintptr_t) p) - ((uintptr_t) array)) / 4) - 1;
    sanity(idx);
    if (!isTaken(idx)) {
        Debug::panic("freeing free block, p:%x idx:%d\n",(uint32_t) p,(int32_t) idx);
        return;
    }
    //GC->unmarkBlock(idx);

    int sz = size(idx);

    int leftIndex = left(idx);
    int rightIndex = right(idx);

    if (isAvail(leftIndex)) {
        remove(leftIndex);
        idx = leftIndex;
        sz += size(leftIndex);
    }

    if (isAvail(rightIndex)) {
        remove(rightIndex);
        sz += size(rightIndex);
    }

    makeAvail(idx,sz);
    //printHeap();
}


/*****************/
/* C++ operators */
/*****************/

void MarkAndSweep::markBlock(void *ptr)
{
    // Calculate the index of the block header from the pointer
    uintptr_t index = (((uintptr_t)ptr - (uintptr_t)gheith::array) / sizeof(int)) - 1;

    // Check if the pointer is within the heap bounds
    if (ptr >= gheith::array && ptr < gheith::array + gheith::len * sizeof(int))
    {
        // Check if the block at the index is taken
        if (gheith::isTaken(index))
        {
            marks[index] = true; // Mark the block as reachable
        }
    }
}
void MarkAndSweep::sweep() {
    int i = 0;
    while (i < gheith::len) {
        if (gheith::isTaken(i) && !marks[i]) {
            // Free this block
            int blockSize = gheith::size(i);
            int totalSize = blockSize;
            int startIndex = i;

            // Check left adjacent block
            int leftIndex = gheith::left(i);
            if (gheith::isAvail(leftIndex)) {
                totalSize += gheith::size(leftIndex);
                startIndex = leftIndex;
                gheith::remove(leftIndex);
            }

            // Check right adjacent block
            int rightIndex = gheith::right(i + blockSize);
            if (rightIndex < gheith::len && gheith::isAvail(rightIndex)) {
                totalSize += gheith::size(rightIndex);
                gheith::remove(rightIndex);
            }

            gheith::makeAvail(startIndex, totalSize);
        }
        i += gheith::size(i); // Move to the next block
        marks[i] = false; // Reset mark for the next GC cycle
    }
}

void* operator new(size_t size) {
    void* p =  malloc(size); //ptr to data 
    if (p == 0) Debug::panic("out of memory");
    int block = ((((uintptr_t)p) - ((uintptr_t)gheith::array)) / 4) - 1; //header block 
    Debug::printf("block %d\n", gheith::array[block]);
    //Debug::printf("size? %d\n", gheith::size(block));
    return p;
}

void operator delete(void* p) noexcept {
    return free(p);
}

void operator delete(void* p, size_t sz) {
    return free(p);
}

void* operator new[](size_t size) {
    void* p =  malloc(size);
    if (p == 0) Debug::panic("out of memory");
    return p;
}

void operator delete[](void* p) noexcept {
    return free(p);
}

void operator delete[](void* p, size_t sz) {
    return free(p);
}
