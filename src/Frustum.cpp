#include "Frustum.h"

#include "Common.h"

Frustum::Frustum()
{

}

void Frustum::calculate(mat4& viewMatrix, mat4& projMatrix)
{
    const mat4 &v = viewMatrix;
    const mat4 &p = projMatrix;

    mat4 clipMatrix;

    clipMatrix[0][0] = v[0][0]*p[0][0]+v[0][1]*p[1][0]+v[0][2]*p[2][0]+v[0][3]*p[3][0];
    clipMatrix[1][0] = v[0][0]*p[0][1]+v[0][1]*p[1][1]+v[0][2]*p[2][1]+v[0][3]*p[3][1];
    clipMatrix[2][0] = v[0][0]*p[0][2]+v[0][1]*p[1][2]+v[0][2]*p[2][2]+v[0][3]*p[3][2];
    clipMatrix[3][0] = v[0][0]*p[0][3]+v[0][1]*p[1][3]+v[0][2]*p[2][3]+v[0][3]*p[3][3];
    clipMatrix[0][1] = v[1][0]*p[0][0]+v[1][1]*p[1][0]+v[1][2]*p[2][0]+v[1][3]*p[3][0];
    clipMatrix[1][1] = v[1][0]*p[0][1]+v[1][1]*p[1][1]+v[1][2]*p[2][1]+v[1][3]*p[3][1];
    clipMatrix[2][1] = v[1][0]*p[0][2]+v[1][1]*p[1][2]+v[1][2]*p[2][2]+v[1][3]*p[3][2];
    clipMatrix[3][1] = v[1][0]*p[0][3]+v[1][1]*p[1][3]+v[1][2]*p[2][3]+v[1][3]*p[3][3];
    clipMatrix[0][2] = v[2][0]*p[0][0]+v[2][1]*p[1][0]+v[2][2]*p[2][0]+v[2][3]*p[3][0];
    clipMatrix[1][2] = v[2][0]*p[0][1]+v[2][1]*p[1][1]+v[2][2]*p[2][1]+v[2][3]*p[3][1];
    clipMatrix[2][2] = v[2][0]*p[0][2]+v[2][1]*p[1][2]+v[2][2]*p[2][2]+v[2][3]*p[3][2];
    clipMatrix[3][2] = v[2][0]*p[0][3]+v[2][1]*p[1][3]+v[2][2]*p[2][3]+v[2][3]*p[3][3];
    clipMatrix[0][3] = v[3][0]*p[0][0]+v[3][1]*p[1][0]+v[3][2]*p[2][0]+v[3][3]*p[3][0];
    clipMatrix[1][3] = v[3][0]*p[0][1]+v[3][1]*p[1][1]+v[3][2]*p[2][1]+v[3][3]*p[3][1];
    clipMatrix[2][3] = v[3][0]*p[0][2]+v[3][1]*p[1][2]+v[3][2]*p[2][2]+v[3][3]*p[3][2];
    clipMatrix[3][3] = v[3][0]*p[0][3]+v[3][1]*p[1][3]+v[3][2]*p[2][3]+v[3][3]*p[3][3];

    planes[PLANE_RIGHT].x = clipMatrix[3][0]-clipMatrix[0][0];
    planes[PLANE_RIGHT].y = clipMatrix[3][1]-clipMatrix[0][1];
    planes[PLANE_RIGHT].z = clipMatrix[3][2]-clipMatrix[0][2];
    planes[PLANE_RIGHT].w = clipMatrix[3][3]-clipMatrix[0][3];

    planes[PLANE_LEFT].x = clipMatrix[3][0]+clipMatrix[0][0];
    planes[PLANE_LEFT].y = clipMatrix[3][1]+clipMatrix[0][1];
    planes[PLANE_LEFT].z = clipMatrix[3][2]+clipMatrix[0][2];
    planes[PLANE_LEFT].w = clipMatrix[3][3]+clipMatrix[0][3];

    planes[PLANE_BOTTOM].x = clipMatrix[3][0]+clipMatrix[1][0];
    planes[PLANE_BOTTOM].y = clipMatrix[3][1]+clipMatrix[1][1];
    planes[PLANE_BOTTOM].z = clipMatrix[3][2]+clipMatrix[1][2];
    planes[PLANE_BOTTOM].w = clipMatrix[3][3]+clipMatrix[1][3];

    planes[PLANE_TOP].x = clipMatrix[3][0]-clipMatrix[1][0];
    planes[PLANE_TOP].y = clipMatrix[3][1]-clipMatrix[1][1];
    planes[PLANE_TOP].z = clipMatrix[3][2]-clipMatrix[1][2];
    planes[PLANE_TOP].w = clipMatrix[3][3]-clipMatrix[1][3];

    planes[PLANE_BACK].x = clipMatrix[3][0]-clipMatrix[2][0];
    planes[PLANE_BACK].y = clipMatrix[3][1]-clipMatrix[2][1];
    planes[PLANE_BACK].z = clipMatrix[3][2]-clipMatrix[2][2];
    planes[PLANE_BACK].w = clipMatrix[3][3]-clipMatrix[2][3];

    planes[PLANE_FRONT].x = clipMatrix[3][0]+clipMatrix[2][0];
    planes[PLANE_FRONT].y = clipMatrix[3][1]+clipMatrix[2][1];
    planes[PLANE_FRONT].z = clipMatrix[3][2]+clipMatrix[2][2];
    planes[PLANE_FRONT].w = clipMatrix[3][3]+clipMatrix[2][3];

    for (uint8 i = 0; i < MAX_PLANES; ++i)
        planes[i] = normalize(planes[i]);
}