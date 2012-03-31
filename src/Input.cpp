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

    switch (key)
    {
        case GLFW_KEY_ESC:
            BaseApp::CloseWindow();
            break;
        case 'A':
            _baseApp->GetRenderDevice()->GetCamera()->Move(MOVE_STRAFE_LEFT, 0.0f);
            break;
        case 'D':
            _baseApp->GetRenderDevice()->GetCamera()->Move(MOVE_STRAFE_RIGHT, 0.0f);
            break;
        case 'W':
            _baseApp->GetRenderDevice()->GetCamera()->Move(MOVE_FORWARD, 1.0f);
            break;
        case 'S':
            _baseApp->GetRenderDevice()->GetCamera()->Move(MOVE_BACKWARD, 1.0f);
            break;
        case 'Q':
            _baseApp->GetRenderDevice()->GetCamera()->Move(MOVE_ROTATE_LEFT, 1.0f);
            break;
        case 'E':
            _baseApp->GetRenderDevice()->GetCamera()->Move(MOVE_ROTATE_RIGHT, 1.0f);
            break;
    }

}

void Keyboard::OnKeyRelease(int32 key)
{
    _keyStateMap[key] = false;
}
