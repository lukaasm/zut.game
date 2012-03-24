#version 330 core

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;
in vec3 in_Color;

out vec3 pass_Color;

void main(void)
{
     gl_Position = modelMatrix * viewMatrix * projMatrix vec4(in_Position, 1.0);
     pass_Color = vec3(0.0f, 0.0f, 1.0f);
}