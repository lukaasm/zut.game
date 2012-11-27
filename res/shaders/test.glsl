#vert_start

#version 330 core

#define ATTR_POSITION	0
#define ATTR_TEXCOORD   1
#define ATTR_NORMAL     2
#define ATTR_COLOR      3

uniform mat4 in_MVP; // Proj*View*Model matrix
uniform mat4 in_MV;  // View*Model matrix
uniform mat4 in_N;   // Normal matrix

out vec3 pass_Color;
out vec2 pass_TexCoord;

out vec3 pass_VertexEyePos;
out vec3 pass_VertexEyeNorm;

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;
layout(location = ATTR_NORMAL) in vec3 in_Normal;
layout(location = ATTR_COLOR) in vec3 in_Color;

void main(void)
{
    gl_Position = in_MVP * vec4(in_Position, 1.0);

    pass_Color = in_Color;
    pass_TexCoord = in_TexCoord;

	pass_VertexEyePos = vec3(in_MV * vec4(in_Position, 1.0));
	pass_VertexEyeNorm = normalize((in_N * vec4(in_Normal, 0.0)).xyz);
}

#vert_end

#frag_start

#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

in vec3 pass_VertexEyePos;
in vec3 pass_VertexEyeNorm;

uniform mat4 in_V;
uniform mat4 in_MV;

uniform lowp float textureFlag;
uniform sampler2D textureSampler;

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
    vec4(5.0, 222.0, 50.0, 1.0), // pos
    vec4(0.7, 0.7, 0.7, 1.0), // diff
    vec4(1.0, 1.0, 1.0, 1.0), // spec
    vec4(0.5, 0.5, 0.5, 1.0)  // amb
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
    vec4(0.6, 0.6, 0.6, 1.0),
    vec4(0.7, 0.7, 0.7, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    1.0
);

void main(void)
{
    light0.position = in_V * light0.position;
    vec3 L = normalize(light0.position.xyz - pass_VertexEyePos);
    vec3 E = normalize(pass_VertexEyePos);
    vec3 R = normalize(reflect(L, pass_VertexEyeNorm));
 
    vec4 Ia = frontMaterial.ambient * light0.ambient;

    vec4 Id = frontMaterial.diffuse * light0.diffuse * max(dot(pass_VertexEyeNorm, L), 0.0);
	Id = clamp(Id, 0.0, 1.0);

    vec4 Is = frontMaterial.specular * light0.specular * pow(max(dot(R, E), 0.0), frontMaterial.shininess);
    Is = clamp(Is, 0.0, 1.0);

	//if (pass_Color.rgb == vec3(0.5f, 0.0f, 0.3f))
	//    out_Color = vec4(pass_Color, 1.0);
	//else
	{
		out_Color = textureFlag * texture2D(textureSampler, pass_TexCoord) + (1.0f - textureFlag) * vec4(pass_Color, 1.0f);
		out_Color *= (Ia + Id + Is);
    }
}

#frag_end
