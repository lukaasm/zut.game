#vert_start

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
    pass_Color = in_Color;
    pass_TexCoord = in_TexCoord;
}

#vert_end

#frag_start

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

#frag_end