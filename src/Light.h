#ifndef H_LIGHT
#define H_LIGHT

#include <glm/glm.hpp>

struct DirectionalLight
{
    glm::vec3 Direction;
    glm::vec3 Color;
};

struct PointLight
{
    glm::vec3 Position;
    glm::vec3 Color;

    float Radius;
    float Intensity;
};

#endif
