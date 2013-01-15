#vert_start

#version 330 core

#define ATTR_POSITION   0
#define ATTR_TEXCOORD   1

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;

out vec2 pass_TexCoord;

void main(void)
{
    gl_Position = vec4(in_Position, 1.0f);
    pass_TexCoord = in_TexCoord;                  
}

#vert_end

#frag_start

#version 330 core

in vec2 pass_TexCoord;

uniform sampler2D ColorTexture;     
uniform sampler2D NormalTexture; 
uniform sampler2D DepthTexture; 

uniform vec3 in_CameraPosition;
uniform mat4 in_InvVP;

struct DirectionalLight
{
    vec3 Direction;
    vec3 Color;
};

uniform DirectionalLight in_Light;

out vec4 out_Color;

void main(void)
{
   // if (texture2D(ColorTexture, pass_TexCoord).a < 1.0f)
   //     discard;

    vec3 lightVector = -normalize(in_Light.Direction);
    //compute diffuse light
    vec3 normal = 2.0f * texture2D(NormalTexture, pass_TexCoord).xyz - 1.0f;
    float NdL = max(0, dot(normal, lightVector));
    vec3 diffuseLight = NdL * in_Light.Color.rgb;

    //output the two lights
    out_Color = vec4(diffuseLight.rgb, 1.0f);
}

#frag_end
