/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "BaseApp.h"

#include <cstdlib>
#include <GL/glew.h>
#include <GL/glfw.h>

#include "RenderDevice.h"

BaseApp::BaseApp()
{
    _renderDevice = new RenderDevice();
}

BaseApp::~BaseApp()
{
    delete_ptr(_renderDevice)
}

void BaseApp::_createContext()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // set OGL window render context which will allow us to use specific gl functions
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create our window
    glfwOpenWindow(800, 600, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

    glfwSetWindowSizeCallback(BaseApp::ResizeWindow);
}

void BaseApp::CreateWindow()
{
    _createContext();

    // needed for new context functions
    glewExperimental = true;
    uint32 glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
        exit(EXIT_FAILURE);

    GetRenderDevice()->OnInit();
}

uint32 BaseApp::GetLoopTime() const
{
    uint32 diff = uint32(glfwGetTime()*1000.0f);
    glfwSetTime(0);
    return diff;
}

void BaseApp::Run()
{
    _stop = false;

    while (!Stopped())
    {
        Update(GetLoopTime());
        Render();
    }
}

void BaseApp::Render()
{
    GetRenderDevice()->OnRender();

    glfwSwapBuffers();
}

ResizeCallBack* BaseApp::ResizeCallback = NULL;

void BaseApp::ResizeWindow(int32 width, int32 height)
{
    (*BaseApp::ResizeCallback)(width, height);
}

void BaseApp::_resizeWindow(int32 width, int32 height)
{
    GetRenderDevice()->OnResize(width, height);
}

void BaseApp::Update(const uint32 diff)
{
}

void main()
{
    BaseApp *p = new BaseApp;

    BaseApp::ResizeCallback = new CallBack<BaseApp, void, int32, int32>(p, &BaseApp::_resizeWindow);

    p->CreateWindow();
    p->Run();

    delete_ptr(p)
}
