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
    float depth = texture2D(DepthTexture, pass_TexCoord).x;

    /*vec4 WorldPosition;
    WorldPosition.x = pass_TexCoord.x * 2.0f - 1.0f;
    WorldPosition.y = -(pass_TexCoord.y * 2.0f - 1.0f);
    WorldPosition.z = depth;
    WorldPosition.w = 1.0f;

    WorldPosition = in_InvVP * WorldPosition;
    WorldPosition /= WorldPosition.w;
    */
    vec3 lightVector = -normalize(in_Light.Direction);
    //compute diffuse light
    vec3 normal = 2.0f * texture2D(NormalTexture, pass_TexCoord).xyz - 1.0f;
    float NdL = max(0, dot(normal, lightVector));
    vec3 diffuseLight = NdL * in_Light.Color.rgb;
    /*//reflexion vector
    vec3 reflectionVector = normalize(reflect(lightVector, normal));
    //camera-to-surface vector
    vec3 directionToCamera = normalize(in_CameraPosition - position.xyz);
    //compute specular light
    float specularLight = specularIntensity * pow(clamp(dot(reflectionVector, directionToCamera), 0.0, 1.0), specularPower);
    */
    //output the two lights
    out_Color = vec4(diffuseLight.rgb, 0.0f);
}

#frag_end
