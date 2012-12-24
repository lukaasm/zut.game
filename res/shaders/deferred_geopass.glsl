#vert_start

#version 330 core

#define ATTR_POSITION   0
#define ATTR_TEXCOORD   1
#define ATTR_NORMAL     2

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;
layout(location = ATTR_NORMAL) in vec3 in_Normal;

uniform mat4 in_MVP;
uniform mat4 in_M;

out vec2 pass_TexCoord;
out vec3 pass_Normal;
out vec4 pass_Position;

void main(void)
{
    gl_Position = in_MVP * vec4(in_Position, 1.0);

    pass_TexCoord = in_TexCoord;                  
    pass_Normal = (in_M * vec4(in_Normal, 0.0f)).xyz;

    pass_Position = gl_Position;
}

#vert_end

#frag_start

#version 330 core

in vec2 pass_TexCoord;
in vec3 pass_Normal;
in vec4 pass_Position;

uniform sampler2D Texture;

layout(location = 0) out vec3 out_Color;    
layout(location = 1) out vec3 out_Normal;           

void main(void)
{
    out_Color = texture2D(Texture, pass_TexCoord).xyz;        
    out_Normal = 0.5f * (normalize(pass_Normal) + 1.0f);    

    gl_FragDepth = pass_Position.z / pass_Position.w;
}

#frag_end
