#include "CopyingCollector.h"
#include "debug.h"
#include "blocking_lock.h"
#include "semaphore.h"
#include "atomic.h"
#include "shared.h"
#include "threads.h"
#include "config.h"
#include "process.h"

CopyingCollector::CopyingCollector(void *heapStart, size_t heapBytes)
{
    fromLocation = (uint32_t *)heapStart;
    toLocation = (uint32_t *)((uint8_t *)heapStart + heapBytes / 2);
    sizeOfSpace = heapBytes / 2 / sizeof(uint32_t);
    allocPointer = fromLocation;
    scanPointer = nullptr;
}

CopyingCollector::~CopyingCollector() {}

void *CopyingCollector::allocate(size_t size)
{
    uint32_t requiredBlocks = (size + sizeof(uint32_t) - 1) / sizeof(uint32_t) + 1;

    if (allocPointer + requiredBlocks >= fromLocation + sizeOfSpace)
    {
        garbageCollect();

        if (allocPointer + requiredBlocks >= fromLocation + sizeOfSpace)
        {
            Debug::panic("Out of memory");
        }
    }

    uint32_t *object = allocPointer;
    allocPointer += requiredBlocks;
    *object = requiredBlocks;

    return object + 1;
}

void CopyingCollector::free(void *ptr) {}

void CopyingCollector::beginCollection()
{
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

void *CopyingCollector::copying(void *obj)
{
    if (obj == nullptr)
    {
        return nullptr;
    }

    uint32_t *fromObject = (uint32_t *)obj - 1;

    if (isForwardAddress(fromObject))
    {
        return (void *)getForwardAddress(fromObject);
    }

    uint32_t size = getSize(fromObject);
    uint32_t *toObject = allocPointer;
    allocPointer += size;

    memcpy(toObject, fromObject, size * sizeof(uint32_t));
    setForwardAddress(fromObject, toObject);

    return toObject + 1;
}

void *CopyingCollector::forward(void *obj)
{
    if (obj == nullptr)
    {
        return nullptr;
    }

    uint32_t *fromObject = (uint32_t *)obj - 1;
    return copying(fromObject + 1);
}

bool CopyingCollector::isForwardAddress(uint32_t *obj)
{
    return (*obj & MARK_BIT) != 0;
}

uint32_t CopyingCollector::getForwardAddress(uint32_t *obj)
{
    return *obj & SIZE_MASK;
}

void CopyingCollector::setForwardAddress(uint32_t *obj, uint32_t *newAddress)
{
    *obj = ((uint32_t)newAddress - (uint32_t)fromLocation) | MARK_BIT;
}

uint32_t CopyingCollector::getSize(uint32_t *obj)
{
    return *obj & SIZE_MASK;
}

void CopyingCollector::flip()
{
    uint32_t *temp = fromLocation;
    fromLocation = toLocation;
    toLocation = temp;
    allocPointer = fromLocation;
}

void CopyingCollector::updateReferences()
{
    for (uint32_t *ptr = scanPointer; ptr < allocPointer; ptr += getSize(ptr))
    {
        uint32_t size = getSize(ptr);
        for (uint32_t i = 1; i <= size / sizeof(uint32_t); i++)
        {
            uint32_t *field = ptr + i;
            if (isForwardAddress(field))
            {
                *field = getForwardAddress(field);
            }
        }
    }
}

void CopyingCollector::garbageCollect()
{
    // Perform garbage collection using copying collector algorithm

    // Copy global variables
    for (uint32_t i = 0; i < numGlobalObjects; i++)
    {
        globalObjects[i] = (uint32_t *)copying(globalObjects[i]);
    }

    // Copy static variables
    for (uint32_t i = 0; i < numStaticObjects; i++)
    {
        staticObjects[i] = (uint32_t *)copying(staticObjects[i]);
    }

    // Copy local variables and registers
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
                if (ptr >= fromLocation && ptr < fromLocation + sizeOfSpace)
                {
                    *ptr = (uint32_t)forward((void *)*ptr);
                }
            }

            gheith::activeThreads[i]->saveArea.ebx = (uint32_t)forward((void *)gheith::activeThreads[i]->saveArea.ebx);
            gheith::activeThreads[i]->saveArea.ebp = (uint32_t)forward((void *)gheith::activeThreads[i]->saveArea.ebp);
            gheith::activeThreads[i]->saveArea.esi = (uint32_t)forward((void *)gheith::activeThreads[i]->saveArea.esi);
            gheith::activeThreads[i]->saveArea.edi = (uint32_t)forward((void *)gheith::activeThreads[i]->saveArea.edi);
        }
    }

    scanPointer = toLocation;
    updateReferences();
    flip();
}

void CopyingCollector::endCollection()
{
    for (uint32_t i = 0; i < kConfig.totalProcs; i++)
    {
        if (gheith::activeThreads[i] != nullptr && !gheith::activeThreads[i]->isIdle)
        {
            gheith::block(gheith::BlockOption::CanReturn, [](gheith::TCB *tcb)
                          { tcb->saveArea.no_preempt = 0; });
        }
    }
}