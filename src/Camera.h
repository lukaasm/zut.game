/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_CAMERA
#define H_CAMERA

#include <glm/glm.hpp>

#include "Common.h"

using namespace glm;

class RenderDevice;

enum MoveType
{
    MOVE_FORWARD      = 1,
    MOVE_BACKWARD     = 2,
    MOVE_ROTATE_LEFT  = 3,
    MOVE_ROTATE_RIGHT = 4,
    MOVE_STRAFE_RIGHT = 5,
    MOVE_STRAFE_LEFT  = 6,
};

class Camera
{
    public:
        Camera(RenderDevice*);

        void OnResize();
        void OnUpdate(const uint32);

        void LookAt();

        void Move(MoveType, float);

        mat4 GetProjMatrix() const { return _projMatrix; }
        mat4 GetViewMatrix() const { return _viewMatrix; }

        void RotateX(float);
        void RotateY(float);

    private:
        mat4 _viewMatrix;
        mat4 _projMatrix;

        vec3 _lookAt;
        vec3 _position;
        vec3 _right;
        vec3 _up;

        float _angle;

        RenderDevice* _renderDevice;
};

#endif
