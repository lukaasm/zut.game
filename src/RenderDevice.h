/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RENDERDEVICE
#define H_RENDERDEVICE

#include <glm/glm.hpp>

#include "Common.h"

class Shader;

class RenderDevice
{
    public:
        RenderDevice();

        void Clear(float, float, float, float);

        void OnInit();
        void OnRender();
        void OnResize(int32, int32);
        void OnUpdate(const uint32);

        void SetUniforms(Shader*);

        glm::mat4 GetModelMatrix() const { return _modelMatrix; }
        glm::mat4 GetViewMatrix() const { return _viewMatrix; }
        glm::mat4 GetProjMatrix() const { return _projMatrix; }

    private:
        int32 _width;
        int32 _height;

        glm::mat4 _projMatrix;
        glm::mat4 _viewMatrix;
        glm::mat4 _modelMatrix;
};

#endif
