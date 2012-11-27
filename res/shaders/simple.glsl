#vert_start

#version 330 core

#define ATTR_POSITION	0
#define ATTR_TEXCOORD   1

uniform mat4 in_MVP; // Proj*View*Model matrix

out vec3 pass_Color;
out vec2 pass_TexCoord;

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;

void main(void)
{
    gl_Position = in_MVP * vec4(in_Position, 1.0);

    pass_Color = in_Position;
    pass_TexCoord = in_TexCoord;
}

#vert_end

#frag_start

#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

out vec4 out_Color;

void main(void)
{
    out_Color = vec4(pass_Color, 1.0);
}

#frag_end
