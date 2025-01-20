#include "bsp.h"
#include "poolTest.h"

#define localPrintf printf

/* Functions -----------------------------------------------------------------*/
int main ()
{
    int i;
    
    PoolTest * pUnit;
    
    
    localPrintf("Create u16, 5 slots\n");
    pUnit = new PoolTest(sizeof(uint16_t), 5);
    pUnit->printInfo();
    delete(pUnit);

    localPrintf("Create u16, 300 slots\n");
    pUnit = new PoolTest(sizeof(uint16_t), 300);
    pUnit->printInfo();
    delete(pUnit);
    
    localPrintf("Create string len 7, 5 slots\n");
    pUnit = new PoolTest(sizeof("Hello!"), 5);
    pUnit->printInfo();
    delete(pUnit);

    localPrintf("Create u16 for general testing\n");
    pUnit = new PoolTest(sizeof(uint16_t), 12);
    pUnit->printInfo();

    //Override the in use table/bits
    pUnit->inUseWrite(3, 1);
    pUnit->inUseWrite(4, 1);
    pUnit->inUseWrite(14, 1);
    
    pUnit->printInfo();
    
    //change something
    uint16_t * pThingA = (uint16_t *)pUnit->_malloc();
    uint16_t * pThingB = (uint16_t *)pUnit->_malloc();
    uint16_t * pThingC = (uint16_t *)pUnit->_malloc();
    UNUSED(pThingA);
    UNUSED(pThingB);
    UNUSED(pThingC);

    pUnit->printInfo();

    //overfill
    for(i = 0; i < pUnit->poolSize; i++)
    {
        pUnit->_malloc();
    }
    localPrintf("Overfilled,\n");
    pUnit->printInfo();
    
    
    localPrintf("freeing 2\n");
    pUnit->_free(pThingB);
    pUnit->_free(pThingC);

    pUnit->printInfo();

    //overfill
    for(i = 0; i < 10; i++)
    {
        pUnit->_malloc();
    }
    localPrintf("Overfilled\n");
    
    //empty
    for(i = 0; i < pUnit->poolSize; i++)
    {
        //This could be a private function to clear
        //It's not to promote higher level clearing, and not mucking
        //about in here.  TBD
        pUnit->_free(pUnit->poolData + (i * pUnit->dataWidth));
    }
    localPrintf("_free, called %d times\n", pUnit->poolSize);
    
    pUnit->printInfo();
}