/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "BaseApp.h"

#include <cstdlib>
#include <GL/glew.h>
#include <GL/glfw.h>

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

    //glfwSetWindowSizeCallback(resizeCallback);

    //log_basic("Successfully created window with OpenGL context: %s", glGetString(GL_VERSION));
}

void BaseApp::ResizeWindow(int32 width, int32 height)
{

}

void BaseApp::CreateWindow()
{
    _createContext();

    // needed for new context functions
    glewExperimental = true;
    uint32 glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
        exit(EXIT_FAILURE);
}

void BaseApp::Run()
{
    _stop = false;
    while (!Stopped())
    {
        glfwSwapBuffers();
    }
}

void main()
{
    BaseApp program;
    program.CreateWindow();
    program.Run();
}
