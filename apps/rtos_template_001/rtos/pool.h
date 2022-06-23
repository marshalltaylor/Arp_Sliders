#ifndef POOL_H
#define POOL_H

#include <stdint.h>

//in poolTest.cpp
extern void test_pool(void);

template <class T>
class Pool
{
public:
    Pool(int size);
    T *palloc(void);
    bool free(T *p);//void free(T *p);
    float used(void);

//private:
    void inUseWrite(int index, bool used);
    bool inUseRead(int index);
    int getIndexFromPointer(T *p);
    uint8_t * inUseBits;
    T * poolData;
    int sz;
};

#endif
