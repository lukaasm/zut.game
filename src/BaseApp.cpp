/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "BaseApp.h"

#include <cstdlib>
#include <GL/glew.h>
#include <GL/glfw.h>

#include "Grid.h"
#include "Input.h"
#include "RenderDevice.h"

#define UPDATE_INTERVAL 50

BaseApp::BaseApp()
{
    _grid = NULL;

    _keyboard = new Keyboard(this);
    _renderDevice = new RenderDevice();
}

BaseApp::~BaseApp()
{
    delete_ptr(BaseApp::CloseCallback)
    delete_ptr(BaseApp::ResizeCallback)

    delete_ptr(_grid)
    delete_ptr(_keyboard)
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
    glfwOpenWindow(1024, 768, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
}

void BaseApp::CreateWindow()
{
    _createContext();

    // needed for new context functions
    glewExperimental = true;
    uint32 glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
        exit(EXIT_FAILURE);

    Init();
}

void BaseApp::CreateCallBacks()
{
    BaseApp::ResizeCallback = new ResizeCallBack(this, &BaseApp::_resizeWindow);
    glfwSetWindowSizeCallback(BaseApp::ResizeWindow);

    BaseApp::CloseCallback = new CloseCallBack(this, &BaseApp::_closeWindow);
    glfwSetWindowCloseCallback(BaseApp::CloseWindow);

    GetKeyboard()->CreateCallBacks();
}

void BaseApp::Init()
{
    GetRenderDevice()->OnInit();

    _grid = new Grid(GetRenderDevice());
}

void BaseApp::Run()
{
    _stop = false;

    while (!Stopped())
    {
        PerformUpdate();

        Render();

        glfwSwapBuffers();
    }

    glfwTerminate();
}

void BaseApp::Render()
{
    GetRenderDevice()->Clear(0.0f, 0.0f, 0.0f, 0.0f);

    GetRenderDevice()->OnRender();

    _grid->OnRender();
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

CloseCallBack* BaseApp::CloseCallback = NULL;

int BaseApp::CloseWindow()
{
    (*BaseApp::CloseCallback)();
    return 0;
}

void BaseApp::_closeWindow()
{
    _stop = true;
}

void BaseApp::Update(const uint32 diff)
{
    GetRenderDevice()->OnUpdate(diff);
}

void BaseApp::PerformUpdate()
{
    const uint32 diff = uint32(glfwGetTime()*1000.0f);
    if (diff >= UPDATE_INTERVAL)
    {
        Update(diff);
        glfwSetTime(0);
    }
}

int main()
{
    BaseApp *p = new BaseApp;

    p->CreateWindow();
    p->CreateCallBacks();
    p->Run();

    delete_ptr(p)

    return 0;
}
