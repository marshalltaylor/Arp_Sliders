#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* References ----------------------------------------------------------------*/
#define CMD_TABLE_MAX_SIZE 10

struct commandInfo_t
{
    const char * cmd;
    const char * comment;
    const char * helpDetail;
    int (*fnptr)(int argc, char *argv[]);
};

//class CommandBase
//{
//public:
//    virtual CommandBase(void){};
//    virtual int run(void);
//}

class CommandTable
{
public:
    CommandTable(void);
    int run(int argc, char *argv[]);
    void help(void);
private:
    commandInfo_t * cmds[CMD_TABLE_MAX_SIZE];
    int cmdLen;
};

//RTOS test commands
extern commandInfo_t statCmd;
extern commandInfo_t bitCmd;
extern commandInfo_t delayCmd;
extern commandInfo_t helloCmd;
extern commandInfo_t xqCmd;

//System control
extern commandInfo_t logCmd;

//Perhiphreal
extern commandInfo_t spiCmd;
extern commandInfo_t adcCmd;
extern commandInfo_t midiCmd;

//Settings (mvc)
extern commandInfo_t settingsCmd;

#endif// COMMANDS_H_INCLUDED
