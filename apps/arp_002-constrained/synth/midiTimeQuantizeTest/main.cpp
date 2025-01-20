#include "bsp.h"

#include "midiTimeQuantize.h"
#include "midiTimeQuantizeTest.h"

#define TESTLENGTH 30

const int32_t testTimes[TESTLENGTH] =
{
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 32, 31,
    32, 33, 34, 35, 36, 37, 0, -14, -13, -12,
};

QCalculator qCalc;


/* Functions -----------------------------------------------------------------*/
static void testFromTable()
{
    printf("Run the test times through the levels\n");
    int32_t qLive[TESTLENGTH];
    int32_t q0[TESTLENGTH];
    int32_t q1[TESTLENGTH];
    int32_t q2[TESTLENGTH];
    int32_t q3[TESTLENGTH];
    int32_t q4[TESTLENGTH];

    qCalc.setEnable(false);
    for(int i = 0; i < TESTLENGTH; i++)
    {
        qLive[i] = qCalc.quantize(testTimes[i]);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(0);
    for(int i = 0; i < TESTLENGTH; i++)
    {
        q0[i] = qCalc.quantize(testTimes[i]);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(1);
    for(int i = 0; i < TESTLENGTH; i++)
    {
        q1[i] = qCalc.quantize(testTimes[i]);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(2);
    for(int i = 0; i < TESTLENGTH; i++)
    {
        q2[i] = qCalc.quantize(testTimes[i]);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(3);
    for(int i = 0; i < TESTLENGTH; i++)
    {
        q3[i] = qCalc.quantize(testTimes[i]);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(4);
    for(int i = 0; i < TESTLENGTH; i++)
    {
        q4[i] = qCalc.quantize(testTimes[i]);
    }

    printf("i    ,  live,    q0,    q1,    q2,    q3,    q4\n");
    for(int i = 0; i < TESTLENGTH; i++)
    {
        printf("%05d, %05d, %05d, %05d, %05d, %05d, %05d\n",
                i,
                qLive[i],
                q0[i],
                q1[i],
                q2[i],
                q3[i],
                q4[i]
                );
    }
}

static void testRange(int start, int end)
{
    printf("Run the test times through the range\n");
    int range = end - start;
    int32_t * qLive = (int32_t*)malloc(range * sizeof(int32_t));
    int32_t * q0 = (int32_t*)malloc(range * sizeof(int32_t));
    int32_t * q1 = (int32_t*)malloc(range * sizeof(int32_t));
    int32_t * q2 = (int32_t*)malloc(range * sizeof(int32_t));
    int32_t * q3 = (int32_t*)malloc(range * sizeof(int32_t));
    int32_t * q4 = (int32_t*)malloc(range * sizeof(int32_t));

    qCalc.setEnable(false);
    for(int i = start; i < end; i++)
    {
        qLive[i] = qCalc.quantize(i);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(0);
    for(int i = start; i < end; i++)
    {
        q0[i] = qCalc.quantize(i);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(1);
    for(int i = start; i < end; i++)
    {
        q1[i] = qCalc.quantize(i);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(2);
    for(int i = start; i < end; i++)
    {
        q2[i] = qCalc.quantize(i);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(3);
    for(int i = start; i < end; i++)
    {
        q3[i] = qCalc.quantize(i);
    }

    qCalc.setEnable(true);
    qCalc.setDivisorPower(4);
    for(int i = start; i < end; i++)
    {
        q4[i] = qCalc.quantize(i);
    }

    printf("i    ,  live,    q0,    q1,    q2,    q3,    q4\n");
    for(int i = start; i < end; i++)
    {
        printf("%05d, %05d, %05d, %05d, %05d, %05d, %05d\n",
                i,
                qLive[i],
                q0[i],
                q1[i],
                q2[i],
                q3[i],
                q4[i]
                );
    }
}

int main ()
{
    //testFromTable();
    UNUSED(testFromTable);
    testRange(-50, 200);
}
