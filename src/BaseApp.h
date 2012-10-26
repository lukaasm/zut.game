/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_BASEAPP
#define H_BASEAPP

#include "CallBack.h"
#include "Common.h"

class BaseApp;
class Keyboard;
class RenderDevice;

typedef CallBack<BaseApp, void, int32, int32 > ResizeCallBack;
typedef CallBack<BaseApp, void > CloseCallBack;

class BaseApp
{
    public:
        BaseApp();
        ~BaseApp();

        void CreateWindow();
        void CreateCallBacks();

        void PerformUpdate();

        void Init();
        void Run();

        void OnRender();
        void OnUpdate(const uint32);

        static ResizeCallBack *ResizeCallback;
        static void ResizeWindow(int32, int32);
        void _resizeWindow(int32, int32);

        static CloseCallBack *CloseCallback;
        static int CloseWindow();
        void _closeWindow();

        bool Stopped() const { return _stop; }

        Keyboard* GetKeyboard() const { return _keyboard; }
        RenderDevice* GetRenderDevice() const { return _renderDevice; }

    private:
        void _createContext();

        bool _stop;

        Keyboard* _keyboard;
        RenderDevice* _renderDevice;
};

#endif
