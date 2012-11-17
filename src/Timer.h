#ifndef H_TIMER
#define H_TIMER

#include "GL/glfw.h"

#include "Common.h"

struct Timer
{
    void Start(uint32 time)
    {
        start = uint32(glfwGetTime()*1000.0f);
        end = start + time;
    }

    bool Passed()
    {
        return uint32(glfwGetTime()*1000.0f) > end;
    }

    uint32 start;
    uint32 end;
};

#endif
