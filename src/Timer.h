#ifndef H_TIMER
#define H_TIMER

#include "Common.h"

struct Timer
{
    Timer() {}

    void Start(uint32 time);
    bool Passed();

    double Elapsed();

    uint32 start;
    uint32 end;
};

#endif
