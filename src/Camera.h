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
    MOVE_ROTATEright = 0x008,
    MOVE_STRAFEright = 0x010,
    MOVE_STRAFE_LEFT  = 0x020,
    MOVEupWARD       = 0x040,
    MOVE_DOWNWARD     = 0x080,
    MOVE_ROTATEup    = 0x100,
    MOVE_ROTATE_DOWN  = 0x200,
};

class Camera
{
    public:
        Camera();

        void AddMoveType(MoveType);
        void ClearMoveType(MoveType);

        void OnResize(int32, int32);
        void OnUpdate(const uint32);

        void LookAt();

        void Move(MoveType, float);

        mat4 GetProjMatrix() const { return projMatrix; }
        mat4 GetViewMatrix() const { return viewMatrix; }

    private:
        mat4 viewMatrix;
        mat4 projMatrix;

        vec3 lookAt;
        vec3 position;
        vec3 right;
        vec3 up;

        MoveType moveFlags;
};

#endif
