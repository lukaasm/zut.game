/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_INPUT
#define H_INPUT

#include "Common.h"

#include "CallBack.h"

class BaseApp;
class Keyboard;

typedef CallBack<Keyboard, void, int32 > KeyStateCallBack;

class Keyboard
{
    public:
        Keyboard(BaseApp*);
        ~Keyboard();

        void CreateCallBacks();
        bool IsKeyPressed(int32);

        static KeyStateCallBack* KeyPressCallBack;
        static KeyStateCallBack* KeyReleaseCallBack;
        static void OnKeyState(int32, int32);

        void OnKeyPress(int32);
        void OnKeyRelease(int32);

    private:
        BaseApp* _baseApp;
};

namespace Mouse
{

}
#endif
