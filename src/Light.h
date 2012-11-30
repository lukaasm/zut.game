#ifndef H_LIGHT
#define H_LIGHT

#include <glm/glm.hpp>

#define MAX_LIGHTS 2

struct LightSource
{
    LightSource() {}
    LightSource(const LightSource& ls)
    {
        Position = ls.Position;
        Diffuse = ls.Diffuse;
        Specular = ls.Specular;

        ConstantAttenuation = ls.ConstantAttenuation;
        LinearAttenuation = ls.LinearAttenuation;
        QuadraticAttenuation = ls.QuadraticAttenuation;

        SpotCutoff = ls.SpotCutoff;
        SpotExponent = ls.SpotExponent;
        SpotDirection = ls.SpotDirection;
    }

    glm::vec4 Position;
    glm::vec4 Diffuse;
    glm::vec4 Specular;

    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;

    float SpotCutoff;
    float SpotExponent;
    glm::vec3 SpotDirection;
};

#endif
