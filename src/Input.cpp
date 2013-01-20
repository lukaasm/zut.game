/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Input.h"

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "BaseApp.h"
#include "Camera.h"
#include "Config.h"
#include "Common.h"
#include "SceneMgr.h"

Keyboard::Keyboard()
{
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
    auto i = _keyStateMap.find(key);
    if (i != _keyStateMap.end())
        return i->second;
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

    if (key == 'K')
        sSceneMgr->ToggleCamera();
    else if (key == 'O')
        std::cerr << ";e " << sSceneMgr->GetPlayer()->GetPosition().x << " "<< sSceneMgr->GetPlayer()->GetPosition().z << std::endl;
}

void Keyboard::OnKeyRelease(int32 key)
{
    _keyStateMap[key] = false;
}

Mouse::Mouse() {}

ButtonStateCallBack* Mouse::ButtonPressCallBack = nullptr;
ButtonStateCallBack* Mouse::ButtonReleaseCallBack = nullptr;
MouseMotionCallBack* Mouse::MousePosChangeCallBack = nullptr;

void Mouse::CreateCallBacks()
{
    ButtonPressCallBack = new ButtonStateCallBack(this, &Mouse::OnButtonPress);
    ButtonReleaseCallBack = new ButtonStateCallBack(this, &Mouse::OnButtonRelease);

    glfwSetMouseButtonCallback(Mouse::OnButtonState);

    MousePosChangeCallBack = new MouseMotionCallBack(this, &Mouse::OnMouseMotion);
    glfwSetMousePosCallback(Mouse::OnMousePosChange);
}

void Mouse::OnButtonState(int32 button, int32 state)
{
    if (state == GLFW_PRESS)
        (*Mouse::ButtonPressCallBack)(button);
    else
        (*Mouse::ButtonReleaseCallBack)(button);
}

void Mouse::OnMousePosChange(int32 x, int32 y)
{
    (*Mouse::MousePosChangeCallBack)(x, y);
}

void Mouse::OnMouseMotion(int32 x, int32 y)
{
    pos.x = x;
    pos.y = sConfig->GetDefault("height", WINDOW_HEIGHT) - y - 1;
}

void Mouse::OnButtonPress(int32 button)
{
    _buttonStateMap[button] = true;
}

void Mouse::OnButtonRelease(int32 button)
{
    _buttonStateMap[button] = false;
}

bool Mouse::IsButtonPressed(int32 button)
{
    auto i = _buttonStateMap.find(button);
    if (i != _buttonStateMap.end())
        return i->second;
    else
        return false;
}

void Mouse::SetPos(uint32 x, uint32 y)
{
    glfwSetMousePos(x, y);
}
