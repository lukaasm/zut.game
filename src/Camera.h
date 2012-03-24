/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_CAMERA
#define H_CAMERA

#include <glm/glm.hpp>

#include "Common.h"

using namespace glm;

class RenderDevice;

class Camera
{
    public:
        Camera(RenderDevice*);

        void OnResize();
        void OnUpdate(const uint32);

        vec3 &GetPosition() { return _position; }
        vec3 &GetOrientation() { return _orientation; }
        vec3 &GetForward() { return _forward; }

        mat4 GetProjMatrix() const { return _projMatrix; }

    private:
        mat4 _lookAtMatrix;
        mat4 _projMatrix;

        vec3 _position;
        vec3 _orientation;
        vec3 _forward;

        RenderDevice* _renderDevice;
};

#endif
