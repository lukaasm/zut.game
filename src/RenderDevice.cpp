/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "RenderDevice.h"

#include <GL/glfw.h>

RenderDevice::RenderDevice()
{

}

void RenderDevice::OnInit()
{

}

void RenderDevice::OnRender()
{

}

void RenderDevice::OnResize(int32 width, int32 height)
{
    _width = width;
    _height = height;

    glViewport(0, 0, width, height);
}

void RenderDevice::OnUpdate(const uint32 diff)
{

}
