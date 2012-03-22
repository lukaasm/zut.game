/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RENDERDEVICE
#define H_RENDERDEVICE

#include "Common.h"

class RenderDevice
{
    public:
        RenderDevice();

        void OnInit();
        void OnRender();
        void OnResize(int32, int32);
        void OnUpdate(const uint32);

    private:
        int32 _width;
        int32 _height;
};

#endif
