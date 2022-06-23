#include "pool.h"

template <class T>
Pool<T>::Pool(int size)
{
    sz = size;
    poolData = new T[size];
    inUseBits = new uint8_t[(size/8) + 1];
    for(int i = 0; i < (size/8) + 1; i++)
    {
        inUseBits[i] = 0;
    }
}

template <class T>
T *Pool<T>::palloc(void)
{
    for(int i = 0; i < sz; i++)
    {
        if(inUseRead(i)==false)
        {
            //memory is available
            inUseWrite(i, true);
            return &poolData[i];
        }
    }
    //No memory available
    return 0;
}

template <class T>
bool Pool<T>::free(T *p)
{
    //Calculate index
    int index = p - poolData;
    if(inUseRead(index) == 0)
    {
        //Wasn't used
        return false;
    }
    inUseWrite(index, 0);
    return true;
}

template <class T>
float Pool<T>::used(void)
{
    //Count all the used flags
    int flags = 0;
    for(int i = 0; i < sz; i++)
    {
        if(inUseRead(i))
        {
            flags++;
        }
    }
    return 100 * ((float)flags)/(float)sz;
}

//No protection, private functions
template <class T>
void Pool<T>::inUseWrite(int index, bool used)
{
    uint8_t byte = index / 8;
    uint8_t bit = index % 8;
    //Extra variable used for understanding
    uint8_t tempVar = inUseBits[byte];
    tempVar = (tempVar & ~(0x01 << bit)) | (used << bit);
    inUseBits[byte] = tempVar;
}

template <class T>
bool Pool<T>::inUseRead(int index)
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

template class Pool<uint16_t>;

