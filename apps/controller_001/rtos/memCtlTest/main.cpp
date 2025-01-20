#include "bsp.h"
#include "pool.h"
#include "poolTest.h"
#include "memoryController.h"

#define localPrintf printf

#define POOLTESTLENGTH 10
PoolTest pool_16(sizeof(uint16_t), POOLTESTLENGTH);

MemCtl mem;

/* Functions -----------------------------------------------------------------*/
int main ()
{
    uint8_t * a = (uint8_t *)malloc(2);
    UNUSED(a);



    //Assign the pool to the memory shims
    //(Omitting this line causes stdlib malloc)
    mem.setPool(&pool_16);
    
    //Check the pool for emptyness
    pool_16.printInfo();
/*
Acknowledge the difference between operating on the pool, and operationg on the shims
*/
    localPrintf("malloc 3 things and keep track of them\n");
    uint16_t * pThingA = (uint16_t *)mem._malloc(sizeof(uint16_t));
    uint16_t * pThingB = (uint16_t *)mem._malloc(sizeof(uint16_t));
    uint16_t * pThingC = (uint16_t *)mem._malloc(sizeof(uint16_t));
    UNUSED(pThingA);
    UNUSED(pThingB);
    UNUSED(pThingC);
    //Check the pool
    pool_16.printInfo();
    //palloc until overfull
    int i;
    for(i = 0; i < POOLTESTLENGTH; i++)
    {
        mem._malloc(sizeof(uint16_t));
    }
    localPrintf("malloc until full and forget the pointers, created: %d\n", i);
    pool_16.printInfo();
    localPrintf("free two of the objects we are tracking\n");
    mem._free(pThingB);
    mem._free(pThingC);
    pool_16.printInfo();
}