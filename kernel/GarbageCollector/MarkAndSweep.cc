#include "MarkAndSweep.h"
#include "debug.h"

int MarkAndSweep::safe = 0;
int MarkAndSweep::avail = 0;
BlockingLock *MarkAndSweep::heapLock = nullptr;

MarkAndSweep::MarkAndSweep(void *heapStart, size_t bytes)
{
    heap = (uint32_t *)heapStart;
    sizeOfHeap = bytes / sizeof(uint32_t);

    heapLock = new BlockingLock();

    for (uint32_t i = 0; i < sizeOfHeap; i++)
    {
        heap[i] = i + 1;
    }
    heap[sizeOfHeap - 1] = 0;
    avail = 0;
}

MarkAndSweep::~MarkAndSweep()
{
    delete heapLock;
}

MarkAndSweep::MarkAndSweep(void *heapStart, size_t bytes)
{
    heap = (uint32_t *)heapStart;
    sizeOfHeap = bytes / sizeof(uint32_t);

    // Initialize the heap lock
    heapLock = new BlockingLock();

    // Initialize the heap with all memory available
    for (uint32_t i = 0; i < sizeOfHeap; i++)
    {
        heap[i] = 0;
    }
    // heap[sizeOfHeap - 1] = 0;
    avail = 0;
}

MarkAndSweep::~MarkAndSweep()
{
    delete heapLock;
}

void *MarkAndSweep::allocate(size_t size)
{
    uint32_t requiredBlocks = (size + sizeof(uint32_t) - 1) / sizeof(uint32_t) + 1;

    LockGuardP<BlockingLock> guard(heapLock);

    uint32_t *freeBlock = nullptr;
    uint32_t *prevBlock = nullptr;

    for (uint32_t *block = &heap[avail]; block < &heap[sizeOfHeap]; block += (*block & GET_SIZE))
    {
        if ((*block & MARKED) == 0 && (*block & GET_SIZE) >= requiredBlocks)
        {
            freeBlock = block;
            break;
        }
        prevBlock = block;
    }

    if (freeBlock == nullptr)
    {
        garbageCollect();

        for (uint32_t *block = &heap[avail]; block < &heap[sizeOfHeap]; block += (*block & GET_SIZE))
        {
            if ((*block & MARKED) == 0 && (*block & GET_SIZE) >= requiredBlocks)
            {
                freeBlock = block;
                break;
            }
            prevBlock = block;
        }

        if (freeBlock == nullptr)
        {
            Debug::panic("Out of memory");
        }
    }

    uint32_t remainingBlocks = (*freeBlock & GET_SIZE) - requiredBlocks;
    if (remainingBlocks > 0)
    {
        uint32_t *nextBlock = freeBlock + requiredBlocks;
        *nextBlock = remainingBlocks;
    }

    *freeBlock = requiredBlocks | MARKED;
    return freeBlock + 1;
}

// Would we even have free? cuz we freeing during the sweep phase?
void MarkAndSweep::free(void *ptr) {}

void MarkAndSweep::beginCollection()
{
    heapLock->lock();

    bool previousState = Interrupts::disable();

    // we iterate over all the processors
    for (uint32_t i = 0; i < kConfig.totalProcs; i++)
    {
        // check if there's an active thread running on the current processor core.
        if (gheith::activeThreads[i] != nullptr && !gheith::activeThreads[i]->isIdle)
        {
            // if we are here it means that there's a running thread on the current core and it is not idle
            // block call blocks the thread. BlockOption::MustBlock means that the thread must be blocked and cannot return until it is explicitly unblocked.
            // lambda that executes while thread is blocked.
            gheith::block(gheith::BlockOption::MustBlock, [](gheith::TCB *tcb)
                          {
            //Setting no_preempt to 1 indicates that the thread should not be preempted or interrupted during the blocking operation.
               tcb->saveArea.no_preempt = 1; });
        }
    }

    interruptState = previousState;
    //Interrupts::restore(previousState);
}

bool MarkAndSweep::isPointer(uint32_t *field)
{
    // Check if the field points to a valid memory location within the heap
    return (field >= heap && field < heap + sizeOfHeap);
}

void MarkAndSweep::markObject(uint32_t *obj)
{
    if (obj == nullptr || (*obj & MARKED) != 0)
    {
        return;
    }

    *obj |= MARKED;

    uint32_t size = *obj & GET_SIZE;

    // Iterate over the object's fields
    for (uint32_t i = 1; i <= size / sizeof(uint32_t); i++)
    {
        uint32_t *field = obj + i;

        // Check if the field is a pointer to another object
        if (isPointer(field))
        {
            // Recursively mark the referenced object
            markObject((uint32_t *)*field);
        }
    }
}

void MarkAndSweep::sweep()
{
    uint32_t *currentBlock = &heap[avail];

    while (currentBlock < &heap[sizeOfHeap])
    {
        if ((*currentBlock & MARKED) == 0)
        {
            // Block is not marked, add it to the free list
            uint32_t blockSize = *currentBlock & GET_SIZE;
            *currentBlock = blockSize;
            currentBlock += blockSize;
        }
        else
        {
            // Block is marked, unmark it for the next GC cycle
            *currentBlock &= ~MARKED;
            currentBlock += *currentBlock & GET_SIZE;
        }
    }
}

void MarkAndSweep::garbageCollect()
{

    // Perform garbage collection using mark and sweep algorithm

    // Mark Phase:
    // Traverse the object graph starting from the roots and mark all reachable objects
    // Identify the root objects based on your object graph

    // Roots can include:
    // - Global variables
    // - Local variables on the stack
    // - Registers
    // - Static variables

    // Example of marking root objects
    for (uint32_t i = 0; i < kConfig.totalProcs; i++)
    {
        if (gheith::activeThreads[i] != nullptr)
        {
            // Mark the thread's stack
            uint32_t *stackStart = nullptr;
            uint32_t *stackEnd = nullptr;

            if (gheith::activeThreads[i]->isIdle)
            {
                // For idle threads, there is no stack to mark
                continue;
            }
            else
            {
                // For non-idle threads, mark the stack
                stackStart = ((gheith::TCBWithStack *)gheith::activeThreads[i])->stack;
                stackEnd = (uint32_t *)gheith::activeThreads[i]->interruptEsp();
            }

            for (uint32_t *ptr = stackStart; ptr < stackEnd; ptr++)
            {
                if (isPointer(ptr))
                {
                    markObject((uint32_t *)*ptr);
                }
            }

            // Mark the thread's registers
            markObject((uint32_t *)gheith::activeThreads[i]->saveArea.ebx);
            markObject((uint32_t *)gheith::activeThreads[i]->saveArea.ebp);
            markObject((uint32_t *)gheith::activeThreads[i]->saveArea.esi);
            markObject((uint32_t *)gheith::activeThreads[i]->saveArea.edi);
        }
    }

    // Sweep Phase:
    // Reclaim unmarked objects
    sweep();
}

void MarkAndSweep::endCollection()
{
    for (uint32_t i = 0; i < kConfig.totalProcs; i++) {
       if (gheith::activeThreads[i] != nullptr && !gheith::activeThreads[i]->isIdle) {
           gheith::block(gheith::BlockOption::CanReturn, [](gheith::TCB* tcb) {
               // Restore the thread's state
               tcb->saveArea.no_preempt = 0;
           });
       }
   }

   // Restore interrupts
   Interrupts::restore(interruptState);

   // Release the heap lock
   heapLock->unlock();
}