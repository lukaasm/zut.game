#vert_start

#version 330 core

#define ATTR_POSITION 0
#define ATTR_TEXCOORD 1

out vec2 pass_TexCoord;

layout(location = ATTR_POSITION) in vec2 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;

uniform int in_ScreenHWidth;// = 512;
uniform int in_ScreenHHeight;// = 384;

void main()
{
    vec2 EyePos = in_Position - vec2(in_ScreenHWidth, in_ScreenHHeight);
    EyePos /= vec2(in_ScreenHWidth, in_ScreenHHeight);
    gl_Position =  vec4(EyePos, 0.0f, 1.0f);
 
    // UV of the vertex. No special space for this one.
    pass_TexCoord = in_TexCoord;
}

#vert_end

#frag_start

#version 330 core

in vec3 pass_Color;
in vec2 pass_TexCoord;

uniform sampler2D textureSampler;

out vec4 out_Color;

void main(void)
{
    out_Color = texture2D(textureSampler, pass_TexCoord);
    if (out_Color == vec4(1.0f, 0.0f, 0.0f, 1.0f))
        discard;
        
   // out_Color.a = 1.0f;
}

#frag_end
