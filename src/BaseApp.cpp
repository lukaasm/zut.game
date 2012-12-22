/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "BaseApp.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GL/glfw.h>

#include "Camera.h"
#include "Config.h"
#include "Exception.h"
#include "Input.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "SceneMgr.h"
#include "Timer.h"

#define UPDATE_STEP 34

double BaseApp::frameTime = 0;
float BaseApp::fps = 0;

BaseApp::BaseApp() : stop(false)
{
}

BaseApp::~BaseApp()
{
    delete_ptr(BaseApp::CloseCallback)
    delete_ptr(BaseApp::ResizeCallback)
}

void BaseApp::createContext()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW Init failed" << std::endl;
        std::exit(EXIT_FAILURE);
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
        std::cerr << "Error: " << glewGetErrorString(glewInitResult) << std::endl;
        std::exit(EXIT_FAILURE);
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
    try
    {
        sResourcesMgr->OnInit();
        sSceneMgr->OnInit();

        int width, height;
        glfwGetWindowSize(&width, &height);
        resizeWindow(width, height);
    }
    catch (Exception& e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
}

void BaseApp::Run()
{
    Timer frameTimer;
    frameTimer.Start(100);

    uint32 accDiff = 0;
    uint32 frameCount = 0;

    double start = glfwGetTime();
    while (!Stopped())
    {
        ++frameCount;

        double now = glfwGetTime();
        accDiff += uint32(ceil((now - start)*1000.0f));
        
        fps = float(frameCount)*1000.0f / frameTimer.Elapsed();

        if (frameTimer.Passed())
        {
            frameCount = 0;

            frameTime = (now - start) * 1000.0f;
            frameTimer.Start(1000);
        }

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
    sSceneMgr->OnResize(width, height);

    sConfig->Set("width", width);
    sConfig->Set("height", height);
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
    try
    {
        sSceneMgr->OnUpdate(diff);
    }
    catch (Exception& e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
}

void BaseApp::OnRender()
{
    try
    {
        sSceneMgr->OnRender();
    }
    catch (Exception& e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
}

int main()
{
    std::ofstream file("error.log");
    if (file.is_open())
        std::cerr.rdbuf(file.rdbuf());

    sConfig->LoadFile("config.ini");

    BaseApp *app = new BaseApp;

    app->CreateWindow();
    app->CreateCallBacks();

    glfwSetTime(0);

    app->Run();

    delete_ptr(app)

    return 0;
}
