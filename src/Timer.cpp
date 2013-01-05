#include "Timer.h"

#include <GL/glfw.h>

void Timer::Start(uint32 time)
{
    start = uint32(glfwGetTime()*1000.0f);
    end = start + time;
}

bool Timer::Passed()
{
    return uint32(glfwGetTime()*1000.0f) > end;
}

double Timer::Elapsed()
{
    return glfwGetTime()*1000.0f - start;
}
