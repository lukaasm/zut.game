#vert_start

#version 330 core

#define ATTR_POSITION   0
#define ATTR_TEXCOORD   1
#define ATTR_NORMAL     2
#define ATTR_COLOR      3

uniform mat4 in_MVP; // Proj*View*Model matrix
uniform mat4 in_MV;  // View*Model matrix
uniform mat3 in_N;   // Normal matrix

out vec3 pass_Color;
out vec2 pass_TexCoord;

out vec3 pass_Normal;
out vec3 pass_Position;

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;
layout(location = ATTR_NORMAL) in vec3 in_Normal;
layout(location = ATTR_COLOR) in vec3 in_Color;

void main(void)
{
    gl_Position = in_MVP * vec4(in_Position, 1.0);

    pass_Color = in_Color;
    pass_TexCoord = in_TexCoord;

    pass_Normal = in_N * in_Normal;
    pass_Position = (in_MV * vec4(in_Position, 1.0f)).xyz;
}

#vert_end

#frag_start

#version 330 core

const int MAX_LIGHTS = 2;

in vec3 pass_Color;
in vec2 pass_TexCoord;

in vec3 pass_Position;
in vec3 pass_Normal;

out vec4 out_Color;

uniform mat4 in_V;
uniform lowp float textureFlag;
uniform sampler2D textureSampler;

struct LightSource
{
    vec4 Position;
    vec4 Diffuse;
    vec4 Specular;

    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;

    float SpotCutoff;
    float SpotExponent;
    vec3 SpotDirection;
};

uniform LightSource in_Lights[MAX_LIGHTS];

uniform int in_LightSourcesNum;

vec4 scene_ambient = vec4(0.0, 0.0, 0.0, 1.0);

struct material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

material frontMaterial = material(
  vec4(0.2, 0.2, 0.2, 1.0),
  vec4(0.6, 0.6, 0.6, 1.0),
  vec4(0.4, 0.4, 0.4, 1.0),
  9.0
);

void main()
{
    vec3 normalDirection = normalize(pass_Normal);
    vec3 viewDirection = normalize(-pass_Position);
    vec3 lightDirection;
    float attenuation;

    vec3 totalLighting = vec3(scene_ambient) * vec3(frontMaterial.ambient);

    for (int i = 0; i < in_LightSourcesNum; ++i)
    {
        vec4 LightPosition = in_V * in_Lights[i].Position;

        // directional light
        if (in_Lights[i].Position.w == 0.0f)
        {
            attenuation = 1.0;
            lightDirection = normalize(LightPosition.xyz);
        }
        else // point light or spotlight (or other kind of light)
        {
            vec3 positionToLightSource = vec3(LightPosition.xyz - pass_Position);
            float distance = length(positionToLightSource);
            lightDirection = normalize(positionToLightSource);
            attenuation = 1.0 / (in_Lights[i].ConstantAttenuation + in_Lights[i].LinearAttenuation * distance + in_Lights[i].QuadraticAttenuation * distance * distance);

            if (in_Lights[i].SpotCutoff <= 90.0) // spotlight?
            {
                float clampedCosine = max(0.0, dot(-lightDirection, normalize(in_Lights[i].SpotDirection)));
                if (clampedCosine < cos(radians(in_Lights[i].SpotCutoff))) // outside of spotlight cone?
                    attenuation = 0.0;
                else
                    attenuation = attenuation * pow(clampedCosine, in_Lights[i].SpotExponent);
            }
        }

        vec3 diffuseReflection = attenuation * vec3(in_Lights[i].Diffuse) * vec3(frontMaterial.diffuse) * max(0.0, dot(normalDirection, lightDirection));

        vec3 specularReflection;
        if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
            specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
        else // light source on the right side
            specularReflection = attenuation * vec3(in_Lights[i].Specular) * vec3(frontMaterial.specular) * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), frontMaterial.shininess);

        totalLighting = totalLighting + diffuseReflection + specularReflection;
    }

    if (textureFlag == 1.f)
        out_Color = texture2D(textureSampler, pass_TexCoord) * vec4(totalLighting, 1.0);
    else
        out_Color = vec4(totalLighting, 1.0);
}

#frag_end
