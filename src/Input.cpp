/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Input.h"

#include <GL/glfw.h>

#include "BaseApp.h"

Keyboard::~Keyboard()
{
    delete_ptr(KeyPressCallBack)
    delete_ptr(KeyReleaseCallBack)
}

void Keyboard::CreateCallBacks()
{
    KeyPressCallBack = new KeyStateCallBack(this, &Keyboard::OnKeyPress);
    KeyReleaseCallBack = new KeyStateCallBack(this, &Keyboard::OnKeyRelease);

    glfwSetKeyCallback(Keyboard::OnKeyState);
}

bool Keyboard::IsKeyPressed(int32 key)
{
    return bool(glfwGetKey(key) == GLFW_PRESS);
}

KeyStateCallBack* Keyboard::KeyPressCallBack = NULL;
KeyStateCallBack* Keyboard::KeyReleaseCallBack = NULL;

void Keyboard::OnKeyState(int32 key, int32 state)
{
    if (state == GLFW_PRESS)
        (*Keyboard::KeyPressCallBack)(key);
    else
        (*Keyboard::KeyReleaseCallBack)(key);
}

void Keyboard::OnKeyPress(int32 key)
{
    if (key == GLFW_KEY_ESC)
        BaseApp::CloseWindow();
}

void Keyboard::OnKeyRelease(int32 key)
{

}
