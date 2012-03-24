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

        void SetUniforms(Shader*, mat4 = mat4(1.0f));

        Camera* GetCamera() const { return _camera; }

        int32 GetWidth() const { return _width; }
        int32 GetHeight() const { return _height; }

        mat4 GetModelMatrix() const { return _modelMatrix; }
        mat4 GetViewMatrix() const { return _viewMatrix; }
        mat4 GetProjMatrix() const;

    private:
        int32 _width;
        int32 _height;

        mat4 _viewMatrix;
        mat4 _modelMatrix;

        Camera* _camera;
};

#endif
