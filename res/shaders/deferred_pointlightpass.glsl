#vert_start

#version 330 core

#define ATTR_POSITION   0

layout(location = ATTR_POSITION) in vec3 in_Position;

uniform mat4 in_MVP;

out vec4 pass_Position;

void main(void)
{
    gl_Position = in_MVP * vec4(in_Position.xyz, 1.0f);
    pass_Position = gl_Position;          
}

#vert_end

#frag_start

#version 330 core

in vec4 pass_Position;

uniform sampler2D ColorTexture;     
uniform sampler2D NormalTexture; 
uniform sampler2D DepthTexture; 

uniform vec3 in_CameraPosition;
uniform mat4 in_InvVP;

struct PointLight
{
    vec3 Position;
    vec3 Color;
    float Radius;
    float Intensity;
};

uniform PointLight in_Light;

out vec4 out_Color;

void main(void)
{
    vec4 ScreenPosition = pass_Position;
    ScreenPosition.xy = ScreenPosition.xy /= ScreenPosition.w;

    vec2 texCoord = 0.5f * (vec2(ScreenPosition.x, ScreenPosition.y) + 1.0f);
    float depth = texture2D(DepthTexture, texCoord).r;

    vec4 WorldPosition;
    WorldPosition.xy = ScreenPosition.xy;
    WorldPosition.z = depth;
    WorldPosition.w = 1.0f;

    WorldPosition = in_InvVP * WorldPosition;
    WorldPosition /= WorldPosition.w;

    vec3 lightVector = in_Light.Position - WorldPosition.xyz;
    float dist = length(lightVector);
    if (dist > in_Light.Radius)
        discard;

    float attenuation = clamp(1.0f - dist / in_Light.Radius, 0.0f, 1.0f); 

    lightVector = normalize(lightVector); 

    //compute diffuse light
    vec3 normal = 2.0f * texture2D(NormalTexture, texCoord).xyz - 1.0f;
    float NdL = max(0, dot(normal, lightVector));
    vec3 diffuseLight = NdL * in_Light.Color.rgb;

    //output the two lights
    out_Color = attenuation * in_Light.Intensity * vec4(diffuseLight.rgb, 0.0);
}

#frag_end
