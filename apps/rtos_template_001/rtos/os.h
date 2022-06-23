#ifndef OS_H
#define OS_H

#ifdef __cplusplus
 extern "C" {
#endif

#define UNUSED(x) (void)x

#define TEST_EVENT_0	( 1 << 0 )
#define TEST_EVENT_1	( 1 << 1 )

//Local functions
void osInit(void);
void vConfigureTimerForRunTimeStats( void );

//Ext Functions
extern void xPortSysTickHandler(void);

/***** Control Interface *****/
typedef struct gameControlInput
{
    int8_t button;
    int8_t state;
} gameControlInput_t;

#ifdef __cplusplus
}
#endif

#endif
