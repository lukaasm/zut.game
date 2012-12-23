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

	vec4 normalData = texture2D(NormalTexture, texCoord);
    vec3 normal = 2.0f * normalData.xyz - 1.0f;

    //float specularPower = normalData.a * 255;
    //float specularIntensity = texture2D(ColorTexture, texCoord).a;

    vec4 position;
    position.xy = ScreenPosition.xy;
    position.z = texture2D(DepthTexture, texCoord).x;
    position.w = 1.0f;

    position = in_InvVP * position;
    position /= position.w;

    vec3 lightVector = in_Light.Position - position.xyz;
    float attenuation = clamp(1.0f - length(lightVector) / in_Light.Radius, 0.0f, 1.0f); 

    lightVector = normalize(lightVector); 
    //compute diffuse light
    float NdL = max(0, dot(normal, lightVector));
    vec3 diffuseLight = NdL * in_Light.Color.rgb;

    //reflexion vector
    //vec3 reflectionVector = normalize(reflect(-lightVector, normal));
    //camera-to-surface vector
    //vec3 directionToCamera = normalize(in_CameraPosition - position.xyz);
    //compute specular light
    //float specularLight = specularIntensity * pow(clamp(dot(reflectionVector, directionToCamera), 0.0, 1.0), specularPower);

    //output the two lights
    out_Color = attenuation * in_Light.Intensity * vec4(diffuseLight.rgb, 0.0);
}

#frag_end
