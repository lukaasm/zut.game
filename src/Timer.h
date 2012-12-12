#ifndef H_TIMER
#define H_TIMER

#include <GL/glfw.h>

#include "Common.h"

struct Timer
{
    Timer() {}

    void Start(uint32 time)
    {
        start = uint32(glfwGetTime()*1000.0f);
        end = start + time;
    }

    bool Passed()
    {
        return uint32(glfwGetTime()*1000.0f) > end;
    }

    double Elapsed() { return glfwGetTime()*1000.0f - start; }

    uint32 start;
    uint32 end;
};

#endif
