#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

uniform lowp float textureFlag;
uniform sampler2D baseTexture;

out vec4 out_Color;

void main(void)
{
    out_Color = textureFlag * texture2D(baseTexture, pass_TexCoord) + (1.0f - textureFlag) * vec4(pass_Color, 1.0f);
}