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
    MOVE_NONE         = 0x00,
    MOVE_FORWARD      = 0x01,
    MOVE_BACKWARD     = 0x02,
    MOVE_ROTATE_LEFT  = 0x04,
    MOVE_ROTATE_RIGHT = 0x08,
    MOVE_STRAFE_RIGHT = 0x10,
    MOVE_STRAFE_LEFT  = 0x20,
};

class Camera
{
    public:
        Camera(RenderDevice*);

        void AddMoveType(MoveType);
        void ClearMoveType(MoveType);

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

        MoveType _moveFlags;

        RenderDevice* _renderDevice;
};

#endif
