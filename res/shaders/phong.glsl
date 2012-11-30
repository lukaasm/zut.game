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
    pass_Position = (in_MV * vec4(in_Position, 0.2f)).xyz;
}

#vert_end

#frag_start

#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

in vec3 pass_Normal;
in vec3 pass_Position;

out vec4 out_Color;

uniform mat4 in_V;

struct Material
{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct LightSource
{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

Material frontMaterial = Material
(
    vec4(0.1f, 0.1f, 0.1f, 1.0f),
    vec4(0.2f, 0.2f, 0.2f, 1.0f),
    vec4(0.5f, 0.5f, 0.5f, 1.0f),
    vec4(0.0f, 0.0f, 0.5f, 1.0f),
    40.0f
);

LightSource light = LightSource
(
    vec4(5.75f, 8.25f, -5.0f, 1.0f),
    vec4(0.0f, 0.0f, 0.0f, 1.0f),
    vec4(1.0f, 1.0f, 1.0f, 1.0f),
    vec4(1.0f, 1.0f, 1.0f, 1.0f)
);

vec4 lightSource(vec3 N, vec3 V)
{
	vec3 lightpos = (in_V * light.position).xyz;

    vec3  L = normalize(lightpos - V);
    vec3  H = normalize(L - V.xyz);

    float NdotL = max(0.0f, dot(N,L));
    float NdotH = max(0.0f, dot(N,H));

    float Idiff = NdotL;
    float Ispec = pow(NdotH, frontMaterial.shininess);

    return 
        frontMaterial.ambient  * light.ambient +
        frontMaterial.diffuse  * light.diffuse  * Idiff +
        frontMaterial.specular * light.specular * Ispec;
}

vec4 lighting( void )
{
    vec3 N = normalize(pass_Normal);

    return
        frontMaterial.emission +
        frontMaterial.ambient * 0.2 +
        lightSource(N, pass_Position);
}

uniform lowp float textureFlag;
uniform sampler2D textureSampler;

void main(void)
{
    if (textureFlag)
        out_Color = texture2D(textureSampler, pass_TexCoord) * lighting();
    else
        out_Color = vec4(pass_Color, 1.0f) * lighting();
}

#frag_end
