#vert_start

#version 330 core

#define ATTR_POSITION   0
#define ATTR_TEXCOORD   1
#define ATTR_NORMAL     2
#define ATTR_TANGENT    3

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;
layout(location = ATTR_NORMAL) in vec3 in_Normal;
layout(location = ATTR_TANGENT) in vec3 in_Tangent;

uniform mat4 in_MVP;
uniform mat4 in_M;

out vec2 pass_TexCoord;
out vec3 pass_Normal;
out vec4 pass_Position;
out vec3 pass_Tangent;

void main(void)
{
    gl_Position = in_MVP * vec4(in_Position, 1.0);

    pass_TexCoord = in_TexCoord;                  
    pass_Normal = (in_M * vec4(in_Normal, 0.0f)).xyz;
    pass_Tangent = (in_M * vec4(in_Tangent, 0.0f)).xyz;
    pass_Position = gl_Position;
}

#vert_end

#frag_start

#version 330 core

in vec2 pass_TexCoord;
in vec3 pass_Normal;
in vec4 pass_Position;
in vec3 pass_Tangent;

uniform float in_NotSkybox;

uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 out_Normal;

vec3 CalculateBumpNormal()
{
    vec3 normal = normalize(pass_Normal);
    vec3 tangent = normalize(pass_Tangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    vec3 BumpMapNormal = texture(NormalTexture, pass_TexCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);

    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main(void)
{
    out_Color = texture2D(DiffuseTexture, pass_TexCoord);
    if (out_Color.a < 0.5f)
        discard;
             
    out_Color.a = in_NotSkybox;
       
    out_Normal.xyz = 0.5f * (CalculateBumpNormal() + 1.0f);    

    gl_FragDepth = pass_Position.z / pass_Position.w;
}

#frag_end
