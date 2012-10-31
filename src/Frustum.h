#ifndef H_FRUSTUM
#define H_FRUSTUM

#include <glm/glm.hpp>

using namespace glm;

enum PlaneSides
{
    PLANE_FRONT  = 0,
    PLANE_BACK   = 1,
    PLANE_TOP    = 2,
    PLANE_BOTTOM = 3,
    PLANE_LEFT   = 4,
    PLANE_RIGHT  = 5,

    MAX_PLANES   = 6
};

typedef vec4 Plane;

class Frustum
{
    public:
        Frustum();

        void Calculate(mat4& viewMatrix, mat4& projMatrix);

    private:
        Plane planes[MAX_PLANES];
};

#endif