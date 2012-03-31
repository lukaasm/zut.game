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
    MOVE_NONE         = 0x000,
    MOVE_FORWARD      = 0x001,
    MOVE_BACKWARD     = 0x002,
    MOVE_ROTATE_LEFT  = 0x004,
    MOVE_ROTATE_RIGHT = 0x008,
    MOVE_STRAFE_RIGHT = 0x010,
    MOVE_STRAFE_LEFT  = 0x020,
    MOVE_UPWARD       = 0x040,
    MOVE_DOWNWARD     = 0x080,
    MOVE_ROTATE_UP    = 0x100,
    MOVE_ROTATE_DOWN  = 0x200,
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
