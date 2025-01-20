#ifndef POOLTEST_H
#define POOLTEST_H

/* Includes -- STD -----------------------------------------------------------*/

/* Includes -- App -----------------------------------------------------------*/
#include "pool.h"

class PoolTest : public Pool
{
public:
    using Pool::Pool;
    void printInfo(void);
};

#endif
