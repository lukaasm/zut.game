#vert_start

#version 330 core

#define ATTR_POSITION 0
#define ATTR_TEXCOORD 1

out vec2 pass_TexCoord;

layout(location = ATTR_POSITION) in vec2 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;

void main()
{
 
    // Output position of the vertex, in clip space
    // map [0..800][0..600] to [-1..1][-1..1]
    vec2 vertexPosition_homoneneousspace = in_Position - vec2(400,300); // [0..800][0..600] -> [-400..400][-300..300]
    vertexPosition_homoneneousspace /= vec2(400,300);
    gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);
 
    // UV of the vertex. No special space for this one.
    pass_TexCoord = in_TexCoord;
}

#vert_end

#frag_start

#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

uniform sampler2D baseTexture;

out vec4 out_Color;

void main(void)
{
    out_Color = texture2D(baseTexture, pass_TexCoord);
    if (out_Color == vec4(1.0f, 0.0f, 0.0f, 1.0f))
        discard;
}

#frag_end
