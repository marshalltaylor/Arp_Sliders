#ifndef CUSTOMMATH_H_INCLUDED
#define CUSTOMMATH_H_INCLUDED

float power(float x, int y);

#define BSPPRINTF_FLOAT(x) \
{ \
    int lhs = (int)x; \
    char c = ' '; \
    if(lhs < 0) \
    { \
        lhs *= -1; \
    } \
    int rhs = (int)(1000000.0*(x-(int)x)); \
    if(rhs < 0) \
    { \
        rhs *= -1; \
        c = '-'; \
    } \
    bspPrintf("Float value: %c%d.%06d\n", c, lhs, rhs); \
} \

#endif //CUSTOMMATH_H_INCLUDED
