#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

uniform lowp float textureFlag;
uniform sampler2D baseTexture;

out vec4 out_Color;

void main(void)
{
    out_Color = texture2D(baseTexture, pass_TexCoord);
    if (out_Color == vec4(1.0f, 0.0f, 0.0f, 1.0f))
        discard;
}