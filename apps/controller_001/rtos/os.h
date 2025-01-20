#ifndef OS_H
#define OS_H

#include "FreeRTOS.h"
#include "task.h"

#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "semphr.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define UNUSED(x) (void)x

#define TEST_EVENT_0	( 1 << 0 )
#define TEST_EVENT_1	( 1 << 1 )

//For argc/v
#define CMDBUFFERSIZE 128

//Bit definitions:
// 0x80 :
// 0x40 :
// 0x20 : Set for running program in taskConsole
// 0x10 : Set when collecting gets() operation
// 0x08 : Set to disable MVC
// 0x04 :
// 0x02 :
// 0x01 :
extern EventGroupHandle_t xTestEventGroup;

extern QueueHandle_t controlQueue;

extern SemaphoreHandle_t xSerCtrl;
extern SemaphoreHandle_t xInputString;

extern char cmdBuffer[];
extern char cmdActive[];

//Local functions
void osInit(void);
void vConfigureTimerForRunTimeStats( void );

//Ext Functions
extern void xPortSysTickHandler(void);

#ifdef __cplusplus
}
#endif

#endif
