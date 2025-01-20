/* Includes -- App -----------------------------------------------------------*/
#include <stdio.h>

/* Includes -- Test ----------------------------------------------------------*/
#include "poolTest.h"

/* References ----------------------------------------------------------------*/
#define SAFETY_LIMIT 10000 //Max index in case of unterminated loops

char debugStr[] = "pool";

//static uint32_t subpulses = 0;
///* Functions -----------------------------------------------------------------*/
void PoolTest::printInfo(void)
{
    printf("[%-10s] Info on poolData @ %p\n", debugStr, (void*)poolData);
    printf("[%-10s]   Bytes in type: %d\n", debugStr, dataWidth);
    printf("[%-10s]   number of slots: %d\n", debugStr, poolSize);
    printf("[%-10s]   slots used: %d (%.2f%%)\n", debugStr, slotsUsed(), 100*(float)slotsUsed()/poolSize);
    printf("[%-10s]   Total bytes of data: %d\n", debugStr, poolSize * dataWidth);
    printf("[%-10s]   Slot bits: ", debugStr);
    for(int i = 0; i < poolSize; i++)
    {
        printf("%d", inUseRead(i));
    }
    printf("\n");
    
    //Errors
    poolErrorStats_t * pErr = getErrorStats();
    printf("[%-10s]   Errors, palloc: %d, pfree: %d\n", debugStr, pErr->failedPalloc, pErr->failedPfree);

}
