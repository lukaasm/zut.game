/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "BaseApp.h"

#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>

#include "Camera.h"
#include "Config.h"
#include "Input.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "SceneMgr.h"

#define UPDATE_STEP 15

BaseApp::BaseApp() : stop(false)
{
    renderDevice = new RenderDevice();
}

BaseApp::~BaseApp()
{
    delete_ptr(BaseApp::CloseCallback)
    delete_ptr(BaseApp::ResizeCallback)

    delete_ptr(renderDevice)
}

void BaseApp::createContext()
{
    if (!glfwInit())
    {
        std::cout << "GLFW Init failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // set OGL window render context which will allow us to use specific gl functions
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create our window
    int width = sConfig->GetDefault("width", 800);
    int height = sConfig->GetDefault("height", 600);

    glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
    glfwSetWindowTitle("WastedProject by lukaasm");
}

void BaseApp::CreateWindow()
{
    createContext();

    // needed for new context functions
    glewExperimental = true;
    uint32 glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
    {
        std::cout << "Error: " << glewGetErrorString(glewInitResult) << std::endl;
        exit(EXIT_FAILURE);
    }

    Init();
}

void BaseApp::CreateCallBacks()
{
    BaseApp::ResizeCallback = new ResizeCallBack(this, &BaseApp::resizeWindow);
    glfwSetWindowSizeCallback(BaseApp::ResizeWindow);

    BaseApp::CloseCallback = new CloseCallBack(this, &BaseApp::closeWindow);
    glfwSetWindowCloseCallback(BaseApp::CloseWindow);

    sKeyboard->CreateCallBacks(this);
}

void BaseApp::Init()
{
    sResourcesMgr->OnInit();
    sSceneMgr->OnInit();

    GetRenderDevice()->OnInit();
}

void BaseApp::Run()
{
    uint32 accDiff = 0;
    double start = glfwGetTime();
    while (!Stopped())
    {
        double now = glfwGetTime();
        accDiff += uint32(ceil((now - start)*1000.0f));
        start = now;

        while (accDiff >= UPDATE_STEP)
        {
            OnUpdate(UPDATE_STEP);
            accDiff -= UPDATE_STEP;
        }

        OnRender();

        glfwSwapBuffers();
    }

    glfwTerminate();
}

ResizeCallBack* BaseApp::ResizeCallback = nullptr;

void BaseApp::ResizeWindow(int32 width, int32 height)
{
    (*BaseApp::ResizeCallback)(width, height);
}

void BaseApp::resizeWindow(int32 width, int32 height)
{
    GetRenderDevice()->OnResize(width, height);
    sSceneMgr->OnResize(width, height);
}

CloseCallBack* BaseApp::CloseCallback = nullptr;

int BaseApp::CloseWindow()
{
    (*BaseApp::CloseCallback)();
    return 0;
}

void BaseApp::closeWindow()
{
    stop = true;
}

void BaseApp::OnUpdate(const uint32 diff)
{
    sSceneMgr->OnUpdate(diff);

    GetRenderDevice()->OnUpdate(diff);
}

void BaseApp::OnRender()
{
    GetRenderDevice()->OnRenderStart();
    sSceneMgr->OnRender(GetRenderDevice());
    GetRenderDevice()->OnRenderEnd();
}

int main()
{
    sConfig->LoadFile("config.ini");

    BaseApp *app = new BaseApp;

    app->CreateWindow();
    app->CreateCallBacks();

    glfwSetTime(0);

    app->Run();

    delete_ptr(app)

    return 0;
}
