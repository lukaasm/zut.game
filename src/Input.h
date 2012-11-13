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

typedef CallBack<Keyboard, void, int32 > KeyStateCallBack;

class Keyboard
{
    SINGLETON(Keyboard)

    public:
        ~Keyboard();

        void CreateCallBacks(BaseApp*);
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

        BaseApp* _baseApp;
};

#define sKeyboard Singleton<Keyboard>::Instance()

namespace Mouse
{

}
#endif
