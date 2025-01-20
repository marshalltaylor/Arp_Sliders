/*******************************************************************************
memoryController.h

Provides an alternate implementation of malloc/free that can be switched between
pools and stdlib.

While not strictly necessary, it enables testing and organization.

App will
create MemCtl object
Optionally,
  create pool object, or other implementor of AllocInterface
  assign pool to MemCtl

If no call to setPool is made, the memoryController will use built-in functions

-------------
Design

The memory controller stores a AllocInterface *pAlObj.

The controller generates a default AllocInterface, DefaultMalloc which implements
the pure virtual functions of the interface.

Alternetly, external code can implement AllocInterface, and then be used in place
of the default.

(Note: There's no way to switch off the declaration of the default object for now,
because it is used in the init.  This can easily be removed by starting with NULL,
which would make uninitialized MemCtl not actually allocate the memory and crash
the app.  It's a toss-up

*******************************************************************************/
#ifndef MEMORYCONTROLLER_H
#define MEMORYCONTROLLER_H

/* Includes -- STD -----------------------------------------------------------*/
#include <stdlib.h>

/* Includes -- App -----------------------------------------------------------*/
#include "bsp.h"

class AllocInterface
{
public:
    AllocInterface(void){};
    virtual void * _malloc(int size) = 0;
    virtual void _free(void * p) = 0;
};

class DefaultMalloc : public AllocInterface
{
public:
    using AllocInterface::AllocInterface;
    void * _malloc(int size){return malloc(size);};
    void _free(void * p){free(p);};
};

class MemCtl
{
public:
    MemCtl(void);
    void setPool(AllocInterface *pInput);
    void *_malloc(int size);
    void _free(void *pInput);
private:
    AllocInterface *pAlObj;
};

#endif
