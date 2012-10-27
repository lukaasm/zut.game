#version 330 core

#define ATTR_POSITION	0
#define ATTR_TEXCOORD   1
#define ATTR_COLOR		3

uniform mat4 mvpMatrix;

out vec3 pass_Color;
out vec2 pass_TexCoord;

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;
layout(location = ATTR_COLOR) in vec3 in_Color;

void main(void)
{
    gl_Position = mvpMatrix * vec4(in_Position, 1.0);
    pass_Color = in_Position;
    pass_TexCoord = in_TexCoord;
}