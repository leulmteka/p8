#include "MarkAndCompact.h"
#include "debug.h"

int MarkAndCompact::safe = 0;
int MarkAndCompact::avail = 0;
BlockingLock *MarkAndCompact::heapLock = nullptr;

MarkAndCompact::MarkAndCompact(void *heapStart, size_t heapBytes)
{
    heap = (uint32_t *)heapStart;
    sizeOfHeap = heapBytes / sizeof(uint32_t);
    compactPointer = nullptr;

    heapLock = new BlockingLock();

    for (uint32_t i = 0; i < sizeOfHeap; i++)
    {
        heap[i] = 0;
    }
}

MarkAndCompact::~MarkAndCompact()
{
    delete heapLock;
}

void *MarkAndCompact::allocate(size_t size)
{
    uint32_t requiredBlocks = (size + sizeof(uint32_t) - 1) / sizeof(uint32_t) + 1;

    LockGuardP<BlockingLock> guard(heapLock);

    uint32_t *freeBlock = nullptr;

    for (uint32_t *block = heap; block < heap + sizeOfHeap; block += (*block & SIZE_MASK))
    {
        if ((*block & MARK_BIT) == 0 && (*block & SIZE_MASK) >= requiredBlocks)
        {
            freeBlock = block;
            break;
        }
    }

    if (freeBlock == nullptr)
    {
        garbageCollect();

        for (uint32_t *block = heap; block < heap + sizeOfHeap; block += (*block & SIZE_MASK))
        {
            if ((*block & MARK_BIT) == 0 && (*block & SIZE_MASK) >= requiredBlocks)
            {
                freeBlock = block;
                break;
            }
        }

        if (freeBlock == nullptr)
        {
            Debug::panic("Out of memory");
        }
    }

    uint32_t blockSize = *freeBlock & SIZE_MASK;
    *freeBlock = requiredBlocks | MARK_BIT;

    return freeBlock + 1;
}

void MarkAndCompact::free(void *ptr) {}

void MarkAndCompact::beginCollection()
{
    heapLock->lock();

    bool previousState = Interrupts::disable();

    for (uint32_t i = 0; i < kConfig.totalProcs; i++)
    {
        if (gheith::activeThreads[i] != nullptr && !gheith::activeThreads[i]->isIdle)
        {
            gheith::block(gheith::BlockOption::MustBlock, [](gheith::TCB *tcb)
                          { tcb->saveArea.no_preempt = 1; });
        }
    }

    Interrupts::restore(previousState);
}

void MarkAndCompact::markObject(uint32_t *obj)
{
    if (obj == nullptr || (*obj & MARK_BIT) != 0)
    {
        return;
    }

    *obj |= MARK_BIT;

    uint32_t size = *obj & SIZE_MASK;

    for (uint32_t i = 1; i <= size / sizeof(uint32_t); i++)
    {
        uint32_t *field = obj + i;
        if (field >= heap && field < heap + sizeOfHeap)
        {
            markObject((uint32_t *)*field);
        }
    }
}

void MarkAndCompact::compact()
{
    compactPointer = heap;

    for (uint32_t *block = heap; block < heap + sizeOfHeap; block += (*block & SIZE_MASK))
    {
        if ((*block & MARK_BIT) != 0)
        {
            uint32_t size = *block & SIZE_MASK;
            if (block != compactPointer)
            {
                memcpy(compactPointer, block, size * sizeof(uint32_t));
                *block = size;
            }
            compactPointer += size;
        }
    }
}
void MarkAndCompact::update()
{
    for (uint32_t *block = heap; block < compactPointer; block += (*block & SIZE_MASK))
    {
        uint32_t size = *block & SIZE_MASK;
        for (uint32_t i = 1; i <= size / sizeof(uint32_t); i++)
        {
            uint32_t *field = block + i;
            if (field >= heap && field < heap + sizeOfHeap)
            {
                *field = (uint32_t)((uint32_t *)*field - heap + heap);
            }
        }
        *block &= SIZE_MASK;
    }
}

bool MarkAndCompact::isMarked(uint32_t *obj)
{
    return (*obj & MARK_BIT) != 0;
}

void MarkAndCompact::garbageCollect()
{
    for (uint32_t i = 0; i < kConfig.totalProcs; i++)
    {
        if (gheith::activeThreads[i] != nullptr)
        {
            uint32_t *stackStart = nullptr;
            uint32_t *stackEnd = nullptr;

            if (gheith::activeThreads[i]->isIdle)
            {
                continue;
            }
            else
            {
                stackStart = ((gheith::TCBWithStack *)gheith::activeThreads[i])->stack;
                stackEnd = (uint32_t *)gheith::activeThreads[i]->interruptEsp();
            }

            for (uint32_t *ptr = stackStart; ptr < stackEnd; ptr++)
            {
                if (ptr >= heap && ptr < heap + sizeOfHeap)
                {
                    markObject((uint32_t *)*ptr);
                }
            }

            markObject((uint32_t *)gheith::activeThreads[i]->saveArea.ebx);
            markObject((uint32_t *)gheith::activeThreads[i]->saveArea.ebp);
            markObject((uint32_t *)gheith::activeThreads[i]->saveArea.esi);
            markObject((uint32_t *)gheith::activeThreads[i]->saveArea.edi);
        }
    }

    compact();
    update();
}

void MarkAndCompact::endCollection()
{
    for (uint32_t i = 0; i < kConfig.totalProcs; i++)
    {
        if (gheith::activeThreads[i] != nullptr && !gheith::activeThreads[i]->isIdle)
        {
            gheith::block(gheith::BlockOption::CanReturn, [](gheith::TCB *tcb)
                          { tcb->saveArea.no_preempt = 0; });
        }
    }

    heapLock->unlock();
}