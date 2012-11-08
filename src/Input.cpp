/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Input.h"

#include <GL/glfw.h>

#include "BaseApp.h"
#include "Common.h"
#include "SceneMgr.h"

Keyboard::Keyboard(BaseApp* app)
{
    _baseApp = app;
}

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
    if (_keyStateMap.find(key) != _keyStateMap.end())
        return _keyStateMap[key];
    else
        return false;
}

MoveType Keyboard::Key2MoveType(int32 key)
{
    switch (key)
    {
        case 'A':
            return MOVE_STRAFE_LEFT;
        case 'D':
            return MOVE_STRAFE_RIGHT;
        case 'W':
            return MOVE_FORWARD;
        case 'S':
            return MOVE_BACKWARD;
        case GLFW_KEY_UP:
            return MOVE_ROTATE_UP;
        case GLFW_KEY_DOWN:
            return MOVE_ROTATE_DOWN;
        case GLFW_KEY_LEFT:
            return MOVE_ROTATE_LEFT;
        case GLFW_KEY_RIGHT:
            return MOVE_ROTATE_RIGHT;
        case GLFW_KEY_SPACE:
            return MOVE_UPWARD;
        case GLFW_KEY_LCTRL:
            return MOVE_DOWNWARD;
        default:
            return MOVE_NONE;
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

    MoveType moveFlag = Keyboard::Key2MoveType(key);
    if (moveFlag != MOVE_NONE)
        sSceneMgr->GetCamera()->AddMoveType(moveFlag);
}

void Keyboard::OnKeyRelease(int32 key)
{
    _keyStateMap[key] = false;

    MoveType moveFlag = Keyboard::Key2MoveType(key);
    if (moveFlag != MOVE_NONE)
        sSceneMgr->GetCamera()->ClearMoveType(moveFlag);
}
