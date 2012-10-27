#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

uniform sampler2D base_texture;

out vec4 out_Color;

void main(void)
{
    out_Color = texture2D(base_texture, pass_TexCoord);
}