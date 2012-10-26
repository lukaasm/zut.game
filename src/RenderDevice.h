/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RENDERDEVICE
#define H_RENDERDEVICE

#include <glm/glm.hpp>

#include "Common.h"

using namespace glm;

class Camera;
class Shader;

class RenderDevice
{
    public:
        RenderDevice();
        ~RenderDevice();

        void Clear(float, float, float, float);

        void OnInit();
        void OnRender();
        void OnResize(int32, int32);
        void OnUpdate(const uint32);

        void SetUniforms(Shader*, mat4&, mat4&, mat4&);

        int32 GetWidth() const { return width; }
        int32 GetHeight() const { return height; }

        void BindVertexArray(uint32);
        void DrawLines(uint32, uint32, uint32);
        void DrawTriangles(uint32, uint32, uint32);

    private:
        int32 width;
        int32 height;
};

#endif
