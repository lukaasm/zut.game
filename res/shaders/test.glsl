#vert_start

#version 330 core

#define ATTR_POSITION	0
#define ATTR_TEXCOORD   1
#define ATTR_NORMAL     2
#define ATTR_COLOR      3

uniform mat4 in_MVP; // Proj*View*Model matrix
uniform mat4 in_M;  // View*Model matrix
uniform mat3 in_N;   // Normal matrix

varying vec3 pass_Color;
varying vec2 pass_TexCoord;

varying vec3 pass_Vpeye;
varying vec3 pass_Vneye;

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;
layout(location = ATTR_NORMAL) in vec3 in_Normal;
layout(location = ATTR_COLOR) in vec3 in_Color;

void main(void)
{
    gl_Position = in_MVP * vec4(in_Position, 1.0);

    pass_Color = in_Color;
    pass_TexCoord = in_TexCoord;

	pass_Vpeye = vec3(in_M * vec4(in_Position, 1.0));
	pass_Vneye = normalize(in_N * in_Normal);
}

#vert_end

#frag_start

#version 330 core

varying vec3 pass_Color;
varying vec2 pass_TexCoord;

varying vec3 pass_Vpeye; // fragment position in eye coords
varying vec3 pass_Vneye; // surface normal in eye coords

uniform lowp float textureFlag;
uniform sampler2D baseTexture;

//uniform mat4 in_V;

out vec4 out_Color;

struct DirectionalLight
{
    vec4 position;

	vec4 diffuse;
	vec4 specular;
    vec4 ambient;
};

DirectionalLight light0 = DirectionalLight
(
    vec4(0.0,  1.0,  -1.0, 1.0), // pos
    vec4(0.7,  0.7,  0.7, 1.0), // diff
    vec4(1.0,  1.0,  1.0, 1.0), // spec
    vec4(0.4, 0.4, 0.4, 1.0)    // amb
);

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

Material frontMaterial = Material
(
    vec4(0.3, 0.3, 0.3, 1.0),
    vec4(0.7, 0.7, 0.7, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    35.0
);

void main(void)
{
    vec3 L = normalize(light0.position.xyz - pass_Vpeye);
    vec3 E = normalize(pass_Vpeye);
    vec3 R = normalize(-reflect(L, pass_Vneye));
 
    vec4 Ia = frontMaterial.ambient * light0.ambient;

    vec4 Id = frontMaterial.diffuse * light0.diffuse * max(dot(pass_Vneye, L), 0.0);
	Id = clamp(Id, 0.0, 1.0);

    vec4 Is = frontMaterial.specular * light0.specular * pow(max(dot(R, E), 0.0), frontMaterial.shininess);
    Is = clamp(Is, 0.0, 1.0);

    out_Color = (textureFlag * texture2D(baseTexture, pass_TexCoord) + (1.0f - textureFlag) * vec4(pass_Color, 1.0f)) * (Ia + Id + Is);
}

#frag_end
