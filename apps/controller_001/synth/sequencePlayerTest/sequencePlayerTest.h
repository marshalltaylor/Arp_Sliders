#ifndef SEQUENCETESTDATA_H
#define SEQUENCETESTDATA_H

/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Includes -- App -----------------------------------------------------------*/
#include "sequence.h"

//Flags control the operation, and can be bitwise ORed
// Using TEST_FLAG_DNP causes only the control bits
// to be used, and the data/time ignored
//#define TEST_FLAG_PRINT         0x01
//#define TEST_FLAG_REC           0x02
//#define TEST_FLAG_CIR           0x04
//#define TEST_FLAG_CLR           0x08
//#define TEST_FLAG_RECLAIM       0x10
//#define TEST_FLAG_DNP           0x20 //Do not process
//#define TEST_FLAG_END           0x80
//
//typedef struct
//{
//	uint8_t data;
//	int time;
//	uint8_t flags;
//} testData_t;

//extern sequence_t testSequence001;
//
//class TestSequencePlayer : public SequencePlayer
//{
//public:
//    using SequencePlayer::SequencePlayer;
//    void listSequence(void);
//    void listSequence(bool listHeader, uint32_t start, uint32_t end);
//    void testBasic(void);
//};

#endif
