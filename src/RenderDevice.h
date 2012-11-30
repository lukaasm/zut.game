/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RENDERDEVICE
#define H_RENDERDEVICE

#include <glm/glm.hpp>

#include "Common.h"

class Camera;
class Shader;
class VertexArrayObject;

class RenderDevice
{
    public:
        RenderDevice();
        ~RenderDevice();

        void OnInit();

        void OnRenderStart();
        void OnRenderEnd();
        void OnResize(int32, int32);
        void OnUpdate(const uint32 &);

        int32 GetWidth() const { return width; }
        int32 GetHeight() const { return height; }

        void ActivateTexture(uint32, uint32);

        void DrawLines(VertexArrayObject&);
        void DrawTriangles(VertexArrayObject&);

    private:
        int32 width;
        int32 height;
};

#endif
