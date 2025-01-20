#ifndef TASKLOG_H
#define TASKLOG_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_STR_LENGTH 40
typedef struct strMsg
{
    int8_t id;
    char data[ 41 ];
} strMsg_t;

extern QueueHandle_t logQueue;

void taskLogStart(void * argument);

#ifdef __cplusplus
}
#endif

#endif
