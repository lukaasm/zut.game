/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Camera.h"

#include <GL/glfw.h>
#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderDevice.h"

using namespace glm;

Camera::Camera(RenderDevice* renderDevice)
{
    _renderDevice = renderDevice;

    _position = vec3(-2.0f, -2.0f, 0.0f);
    _lookAt = vec3(0.0f, 0.0f, -1.0f);

    _angle = 0.0f;

    _up = vec3(0.0f, 1.0f, 0.0f);
    _right = vec3(1.0f, 0.0f, 0.0f);
}

void Camera::LookAt()
{
    /*vec3 viewZ = normalize(_position - _lookAt);
    vec3 viewX = normalize(cross(viewZ, _up));
    vec3 viewY = normalize(cross(viewX, viewZ));

    mat4 rotation = mat4(vec4(viewX.x, viewY.x, viewZ.x, 0.0f), vec4(viewX.y, viewY.y, viewZ.y, 0.0f), vec4(viewX.z, viewY.z, viewZ.z, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f));
    mat4 translation = translate(mat4(1.0f), _position*(-1.0f));

    _viewMatrix = rotation * translation;*/

    _viewMatrix = lookAt(_position, _lookAt, _up);
}

void Camera::Move(MoveType type, float angleOrDist)
{
    switch (type)
    {
        case MOVE_FORWARD:
        {
            vec3 direction = normalize(_lookAt - _position);
            _position += direction;
            _lookAt += direction;
            break;
        }
        case MOVE_BACKWARD:
        {
            vec3 direction = normalize(_lookAt - _position);
            _position -= direction;
            _lookAt -= direction;
            break;
        }
        case MOVE_ROTATE_LEFT:
            _angle += 0.005f;
            break;
        case MOVE_ROTATE_RIGHT:
            _angle -= 0.005f;
            break;
        case MOVE_STRAFE_RIGHT:
        {
            _position -= _right*0.5f;
            _lookAt -= _right*0.5f;
            break;
        }
        case MOVE_STRAFE_LEFT:
        {
            _position += _right*0.5f;
            _lookAt += _right*0.5f;
            break;
        }
    }
}

void Camera::RotateX(float angle)
{
//    _direction = normalize(rotateX(_up, angle));
//    _up = cross(_direction, _right);
}

void Camera::RotateY(float angle)
{
  //  _direction = normalize(rotateY(_right, angle));
  //  _right = cross(_direction, _up);
}

void Camera::OnResize()
{
    _projMatrix = perspective(60.0f, float(_renderDevice->GetWidth() / _renderDevice->GetHeight()), 0.1f, 1000.f);
}

void Camera::OnUpdate(const uint32 diff)
{
    //_position += _up*0.005f;
    //_lookAt += _up*0.005f;
}
