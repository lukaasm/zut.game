/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Input.h"

#include <GL/glfw.h>

#include "BaseApp.h"
#include "Config.h"
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

MoveInfo Keyboard::Key2MoveInfo(int32 key)
{
    switch (key)
    {
        case 'A':
            return moveInfos[MOVE_TYPE_STRAFE_LEFT];
        case 'D':
            return moveInfos[MOVE_TYPE_STRAFE_RIGHT];
        case 'W':
            return moveInfos[MOVE_TYPE_FORWARD];
        case 'S':
            return moveInfos[MOVE_TYPE_BACKWARD];
        case GLFW_KEY_LEFT:
            return moveInfos[MOVE_TYPE_ROTATE_LEFT];
        case GLFW_KEY_RIGHT:
            return moveInfos[MOVE_TYPE_ROTATE_RIGHT];
        case GLFW_KEY_SPACE:
            return moveInfos[MOVE_TYPE_UPWARD];
        case GLFW_KEY_LCTRL:
            return moveInfos[MOVE_TYPE_DOWNWARD];
        default:
            return moveInfos[MOVE_TYPE_NONE];
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

    if (key == 'L')
        sConfig->Set("render.textures", sConfig->Get<bool>("render.textures") ? "0" : "1");

    if (key == GLFW_KEY_ESC)
        BaseApp::CloseWindow();
}

void Keyboard::OnKeyRelease(int32 key)
{
    _keyStateMap[key] = false;
}
