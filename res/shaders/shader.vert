#version 330 core

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;
in vec3 in_Color;

out vec3 pass_Color;

void main(void)
{
     gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
     pass_Color = in_Color;
}