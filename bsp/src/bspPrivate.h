#ifndef BSP_PRIVATE_H
#define BSP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

//Used for uart monitoring right now
//extern bspTimerCallback_t timer4TickCallback;

//These force memory allocation
extern uint8_t testDACBuffer[];
//extern uint8_t test_pattern_160x120_data[];
extern uint8_t test_pattern_192x144_data[];
extern uint8_t test_image_192x144_data[];
extern uint8_t custom_test_pattern_192x144_data[];

extern int32_t encoderSteps;

// __attribute__((used)) = {
#ifdef __cplusplus
}
#endif

#endif
