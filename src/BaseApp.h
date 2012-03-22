/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_BASEAPP
#define H_BASEAPP

#include "CallBack.h"
#include "Common.h"

class BaseApp;
class RenderDevice;

typedef CallBack<BaseApp, void, int32, int32 > ResizeCallBack;
typedef CallBack<BaseApp, void, void, void > CloseCallBack;

class BaseApp
{
    public:
        BaseApp();
        ~BaseApp();

        void CreateWindow();

        uint32 GetLoopTime() const;

        void Render();
        void Run();
        void Update(const uint32);

        static ResizeCallBack *ResizeCallback;
        static void ResizeWindow(int32, int32);

        void _resizeWindow(int32, int32);

        bool Stopped() const { return _stop; }
        RenderDevice* GetRenderDevice() const { return _renderDevice; }

    private:
        void _createContext();

        bool _stop;

        RenderDevice* _renderDevice;
};

#endif
