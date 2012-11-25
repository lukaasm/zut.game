#vert_start

#version 330 core

#define ATTR_POSITION	0
#define ATTR_TEXCOORD   1
#define ATTR_NORMAL     2
#define ATTR_COLOR      3

uniform mat4 in_MVP; // Proj*View*Model matrix
uniform mat4 in_MV;  // View*Model matrix
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

	pass_Vpeye = vec3(in_MV * vec4(in_Position, 1.0));
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

out vec4 out_Color;

struct DirectionalLight
{
    vec4 position;

    vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float specularExp;
};

uniform DirectionalLight in_DirectionalLight;

void main(void)
{
    vec3 L = normalize(in_DirectionalLight.position.xyz - pass_Vpeye);
    vec3 E = normalize(pass_Vpeye);
    vec3 R = normalize(-reflect(L, pass_Vneye));
 
    vec4 Ia = vec4(0.01f, 0.01f, 0.01f, 1.0f) * in_DirectionalLight.ambient;

    vec4 Id = vec4(0.01f, 0.01f, 0.01f, 1.0f) * in_DirectionalLight.diffuse * max(dot(pass_Vneye, L), 0.0);
	Id = clamp(Id, 0.0, 1.0);

    vec4 Is = vec4(1.0, 1.0, 1.0, 1.0) * in_DirectionalLight.specular * pow(max(dot(R, E), 0.0), in_DirectionalLight.specularExp);
    Is = clamp(Is, 0.0, 1.0);

    out_Color = (textureFlag * texture2D(baseTexture, pass_TexCoord) + (1.0f - textureFlag) * vec4(pass_Color, 1.0f)) * (Ia + Id + Is);
}

#frag_end
