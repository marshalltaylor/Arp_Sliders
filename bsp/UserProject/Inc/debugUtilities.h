#ifndef DEBUGPINS_H
#define DEBUGPINS_H
#include <stdbool.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define PORTA_OFFSET 0x0000U
#define PORTB_OFFSET 0x0400U
#define PORTC_OFFSET 0x0800U
#define PORTD_OFFSET 0x0C00U
#define PORTE_OFFSET 0x1000U
#define PORTF_OFFSET 0x1400U
#define PORTG_OFFSET 0x1800U
#define PORTH_OFFSET 0x1C00U
#define PORTI_OFFSET 0x2000U
#define PORTJ_OFFSET 0x2400U
#define PORTK_OFFSET 0x2800U

//typedef struct DebugObject
//{
//	uint32_t i;
//	uint32_t param1[32];
//	uint32_t param2[32];
//	uint32_t param3[32];
//	uint32_t param4[32];
//} DebugObject_t;

#define LOG_LENGTH 32
typedef struct logRecord
{
	uint32_t lineNum;
	uint32_t value;
	uint32_t text[10];
} logRecord_t;

typedef struct logRecords
{
	uint32_t last_written;
	logRecord_t record[LOG_LENGTH];
} logRecords_t;

typedef enum SCOPE_COLORS
{
	YELLOW,
	CYAN,
	VIOLET,
	NAVY
} SCOPE_COLORS_T;

void traceWrite(SCOPE_COLORS_T trace, bool state);

void debugLogRecord(uint32_t line, uint32_t value, const char * tenChars);
void debugLogClear(void) __attribute__((used));
void printLog() __attribute__((used));

#ifdef __cplusplus
}
#endif
#endif