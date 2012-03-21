/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_BASEAPP
#define H_BASEAPP

#include "Common.h"

class RenderDevice;

class BaseApp
{
    public:
        void Run();
        void CreateWindow();

        void ResizeWindow(int32, int32);

        bool Stopped() const { return _stop; }
        RenderDevice* GetRenderDevice() const { return _renderDevice; }

    private:
        void _createContext();

        bool _stop;
        RenderDevice* _renderDevice;
};

#endif
