#version 330 core

#define ATTR_POSITION	0
#define ATTR_TEXCOORD   1

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