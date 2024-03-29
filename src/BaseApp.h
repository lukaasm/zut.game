/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_BASEAPP
#define H_BASEAPP

#include "CallBack.h"
#include "Common.h"

class BaseApp;

typedef CallBack<BaseApp, void, int32, int32 > ResizeCallBack;
typedef CallBack<BaseApp, void > CloseCallBack;

class BaseApp
{
    public:
        BaseApp();
        ~BaseApp();

        void CreateWindow();
        void CreateCallBacks();

        void Init();
        void Run();

        void OnRender();
        void OnUpdate(const uint32);

        static double frameTime;
        static float fps;

        static ResizeCallBack *ResizeCallback;
        static void ResizeWindow(int32, int32);

        static CloseCallBack *CloseCallback;
        static int CloseWindow();

        bool Stopped() const { return stop; }

    private:
        void closeWindow();
        void createContext();
        void resizeWindow(int32, int32);

        bool stop;
};

#endif
