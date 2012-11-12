/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Input.h"

#include <GL/glfw.h>

#include "BaseApp.h"
#include "Common.h"
#include "SceneMgr.h"

Keyboard::~Keyboard()
{
    delete_ptr(KeyPressCallBack)
    delete_ptr(KeyReleaseCallBack)
}

void Keyboard::CreateCallBacks(BaseApp* baseApp)
{
    _baseApp = baseApp;

    KeyPressCallBack = new KeyStateCallBack(this, &Keyboard::OnKeyPress);
    KeyReleaseCallBack = new KeyStateCallBack(this, &Keyboard::OnKeyRelease);

    glfwSetKeyCallback(Keyboard::OnKeyState);
}

bool Keyboard::IsKeyPressed(int32 key)
{
    if (_keyStateMap.find(key) != _keyStateMap.end())
        return _keyStateMap[key];
    else
        return false;
}

MoveFlag Keyboard::Key2MoveFlag(int32 key)
{
    switch (key)
    {
        case 'A':
            return moveflags[2];
        case 'D':
            return moveflags[3];
        case 'W':
            return moveflags[0];
        case 'S':
            return moveflags[1];
        case GLFW_KEY_LEFT:
            return moveflags[4];
        case GLFW_KEY_RIGHT:
            return moveflags[5];
        case GLFW_KEY_SPACE:
            return moveflags[6];
        case GLFW_KEY_LCTRL:
            return moveflags[7];
        default:
            return moveflags[8];
    }
}

KeyStateCallBack* Keyboard::KeyPressCallBack = nullptr;
KeyStateCallBack* Keyboard::KeyReleaseCallBack = nullptr;

void Keyboard::OnKeyState(int32 key, int32 state)
{
    if (state == GLFW_PRESS)
        (*Keyboard::KeyPressCallBack)(key);
    else
        (*Keyboard::KeyReleaseCallBack)(key);
}

void Keyboard::OnKeyPress(int32 key)
{
    _keyStateMap[key] = true;

    if (key == GLFW_KEY_ESC)
        BaseApp::CloseWindow();
}

void Keyboard::OnKeyRelease(int32 key)
{
    _keyStateMap[key] = false;
}
