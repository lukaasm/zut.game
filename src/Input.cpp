/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Input.h"

#include <GL/glfw.h>

#include "BaseApp.h"
#include "Camera.h"
#include "RenderDevice.h"

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
    _keyStateMap[key] = true;

    const int32 moveKey[GLFW_KEY_LAST][2] =
    {
        { 'A', MOVE_STRAFE_LEFT  },
        { 'D', MOVE_STRAFE_RIGHT },
        { 'W', MOVE_FORWARD      },
        { 'S', MOVE_BACKWARD     },
        { 'E', MOVE_ROTATE_LEFT  },
        { 'Q', MOVE_ROTATE_RIGHT }
    };

    MoveType moveFlag = MOVE_NONE;
    switch (key)
    {
        case GLFW_KEY_ESC:
            BaseApp::CloseWindow();
            break;
        case 'A':
            moveFlag = MOVE_STRAFE_LEFT;
            break;
        case 'D':
            moveFlag = MOVE_STRAFE_RIGHT;
            break;
        case 'W':
            moveFlag = MOVE_FORWARD;
            break;
        case 'S':
            moveFlag = MOVE_BACKWARD;
            break;
        case 'E':
            moveFlag = MOVE_ROTATE_LEFT;
            break;
        case 'Q':
            moveFlag = MOVE_ROTATE_RIGHT;
            break;
    }

    if (moveFlag != MOVE_NONE)
        _baseApp->GetRenderDevice()->GetCamera()->AddMoveType(moveFlag);
}

void Keyboard::OnKeyRelease(int32 key)
{
    _keyStateMap[key] = false;

    MoveType moveFlag = MOVE_NONE;
    switch (key)
    {
        case GLFW_KEY_ESC:
            BaseApp::CloseWindow();
            break;
        case 'A':
            moveFlag = MOVE_STRAFE_LEFT;
            break;
        case 'D':
            moveFlag = MOVE_STRAFE_RIGHT;
            break;
        case 'W':
            moveFlag = MOVE_FORWARD;
            break;
        case 'S':
            moveFlag = MOVE_BACKWARD;
            break;
        case 'E':
            moveFlag = MOVE_ROTATE_LEFT;
            break;
        case 'Q':
            moveFlag = MOVE_ROTATE_RIGHT;
            break;
    }

    if (moveFlag != MOVE_NONE)
        _baseApp->GetRenderDevice()->GetCamera()->ClearMoveType(moveFlag);
}
