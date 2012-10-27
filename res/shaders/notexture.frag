#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

out vec4 out_Color;

void main(void)
{
    out_Color = vec4(pass_Color, 1.0);
}