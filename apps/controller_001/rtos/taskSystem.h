#ifndef TASKSYSTEM_H
#define TASKSYSTEM_H

#include "bsp.h"

void tSysGetSerialFunctions(comPortInterface_t * interface);

void taskSystemStart(void * argument);

char * gets( char * str );

#endif
