#version 330 core

#define ATTR_POSITION	0
#define ATTR_COLOR		3

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 pass_Color;

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_COLOR) in vec3 in_Color;

void main(void)
{
     gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
     pass_Color = in_Position;
}