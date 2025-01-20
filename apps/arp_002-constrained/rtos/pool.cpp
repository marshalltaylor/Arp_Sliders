#include <stdlib.h>
#include <cstring>
#include "pool.h"

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

Pool::Pool(int dataWidthVal, int poolSizeVal)
{
    poolSize = poolSizeVal;
    if(dataWidthVal % 4 != 0) // Not 32bit aligned
    {
        dataWidthVal = ((dataWidthVal >> 2) + 1) << 2;
    }
    dataWidth = dataWidthVal;
    poolData = (uint8_t*)calloc(dataWidth, poolSize);
    inUseBits = new uint8_t[(poolSize/8) + 1];
    for(int i = 0; i < (poolSize/8) + 1; i++)
    {
        inUseBits[i] = 0;
    }
    memset(&errorStats, 0, sizeof(poolErrorStats_t));
}

Pool::~Pool(void)
{
    free(poolData);
    free(inUseBits);
}

//the pool has pre-defined sizes, but this function
//is needed to fulfill the memoryController.h interface
pool_t Pool::_malloc(int size)
{
    if(size > dataWidth)
    {
        //printf("*** ERROR ***: in pool, requested more bytes than allowed, giving anyway\n");
    }
    return _malloc();
}

pool_t Pool::_malloc(void)
{
    for(int i = 0; i < poolSize; i++)
    {
        if(inUseRead(i)==false)
        {
            //memory is available
            inUseWrite(i, true);
            uint8_t * pSub = poolData + (dataWidth * i);
            //printf("[*****]new @ %p\n", (void*)pSub);
            return pSub;
        }
    }
    //No memory available
    errorStats.failedPalloc++;
    return 0;
}

void Pool::_free(void * p)
{
    //Calculate index
    int index = ((uint8_t*)p - poolData) / dataWidth;
    if(inUseRead(index) == 0)
    {
        //Wasn't used
        errorStats.failedPfree++;
    }
    inUseWrite(index, 0);
}

int Pool::slotsUsed(void)
{
    //Count all the used flags
    int flags = 0;
    for(int i = 0; i < poolSize; i++)
    {
        if(inUseRead(i))
        {
            flags++;
        }
    }
    return flags;
}

poolErrorStats_t * Pool::getErrorStats(void)
{
    return &errorStats;
}

void Pool::clearErrorStats(void)
{
    memset(&errorStats, 0, sizeof(errorStats));
}

//No protection, private functions
void Pool::inUseWrite(int index, bool used)
{
    uint8_t byte = index / 8;
    uint8_t bit = index % 8;
    //Extra variable used for understanding
    uint8_t tempVar = inUseBits[byte];
    tempVar = (tempVar & ~(0x01 << bit)) | (used << bit);
    inUseBits[byte] = tempVar;
}

bool Pool::inUseRead(int index)
{
    uint8_t byte = index / 8;
    uint8_t bit = index % 8;
    if(inUseBits[byte] & (0x01 << bit))
    {
        return true;
    }
    else
    {
        return false;
    }
}

