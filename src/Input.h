/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_INPUT
#define H_INPUT

#include "Common.h"

#include <map>

#include "CallBack.h"

// there is NO enum predefinition, so whole header is needed :p
#include "DynamicObject.h"
#include "Singleton.h"

class BaseApp;
class Keyboard;
class Mouse;

typedef CallBack<Keyboard, void, int32 > KeyStateCallBack;
typedef CallBack<Mouse, void, int32 > ButtonStateCallBack;
typedef CallBack<Mouse, void, int32, int32 > MouseMotionCallBack;

class Keyboard
{
    SINGLETON(Keyboard)

    public:
        ~Keyboard();

        void CreateCallBacks();
        bool IsKeyPressed(int32);

        static MoveInfo Key2MoveInfo(int32);

        static KeyStateCallBack* KeyPressCallBack;
        static KeyStateCallBack* KeyReleaseCallBack;
        static void OnKeyState(int32, int32);

        void OnKeyPress(int32);
        void OnKeyRelease(int32);

        typedef std::map<int32, bool> KeysMap;
        KeysMap& GetKeysMap() { return _keyStateMap; }

    private:
        KeysMap _keyStateMap;
};

#define sKeyboard Singleton<Keyboard>::Instance()

class Mouse
{
    SINGLETON(Mouse)
    public:
        void CreateCallBacks();
        bool IsButtonPressed(int32);

        void OnButtonPress(int32);
        void OnButtonRelease(int32);

        void OnMouseMotion(int32, int32);

        static ButtonStateCallBack* ButtonPressCallBack;
        static ButtonStateCallBack* ButtonReleaseCallBack;
        static void OnButtonState(int32, int32);

        static MouseMotionCallBack* MousePosChangeCallBack;
        static void OnMousePosChange(int32, int32);

        typedef std::map<int32, bool> ButtonsMap;
        ButtonsMap& GetButtonsMap() { return _buttonStateMap; }

        glm::vec2 GetPos() { return pos; }
        void SetPos(uint32 x, uint32 y);

    private:
        ButtonsMap _buttonStateMap;

        glm::vec2 pos;
};

#define sMouse Singleton<Mouse>::Instance()

#endif
