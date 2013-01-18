#vert_start

#version 330 core

#define ATTR_POSITION    0
#define ATTR_TEXCOORD    1

layout(location = ATTR_POSITION) in vec3 in_Position;
layout(location = ATTR_TEXCOORD) in vec2 in_TexCoord;

out vec2 pass_TexCoord;

void main(void)
{
    gl_Position = vec4(in_Position, 1.0);

    pass_TexCoord = in_TexCoord;
}

#vert_end

#frag_start

#version 330 core

in vec2 pass_TexCoord;

uniform sampler2D ColorTexture;
uniform sampler2D LightTexture;
uniform sampler2D DepthTexture;
uniform sampler2D ShadowTexture;

uniform mat4 in_InvVP;
uniform mat4 in_ShadowVP;

out vec4 out_Color;

const mat4 ScaleMatrix = mat4(0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 0.5, 0.0,
                              0.5, 0.5, 0.5, 1.0);

const float blurSize = 1.0/1024.0;

const vec2 poissonDisk[16] = vec2[]
( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

void main(void)
{
    float alpha = texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y)).a;

    // PSEUDO BLUR ! :P
    vec4 diffuseColor = texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y));// * 0.16;
    vec4 light = texture2D(LightTexture, pass_TexCoord);// * 0.16;

    
    float depth = texture2D(DepthTexture, pass_TexCoord).x;

    vec4 WorldPosition;
    WorldPosition.xy = pass_TexCoord.xy * 2.0f - 1.0f;
    WorldPosition.z = depth;
    WorldPosition.w = 1.0f;

    WorldPosition = in_InvVP * WorldPosition;
    //WorldPosition /= WorldPosition.w;

    vec4 ShadowCoord = in_ShadowVP * WorldPosition;
    ShadowCoord.xyz /= ShadowCoord.w;

    ShadowCoord /= 2.0;
	ShadowCoord += 0.5;

    float bias = 0;//0.00001;//0.0005;
    float shadow = 1.0;

    const int m = 10;
    for (int i = 0; i < m; i++)
    {
        if (ShadowCoord.z-bias > texture2D(ShadowTexture, ShadowCoord.xy + poissonDisk[i]/700.0).x)
            shadow -= .6 / m;
    }

    /*if (shadow < 1.0f)
    {
        diffuseColor *= 0.16f;
        diffuseColor += texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y - 4.0*blurSize)) * 0.05;
        diffuseColor += texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y - 3.0*blurSize)) * 0.09;
        diffuseColor += texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y - 2.0*blurSize)) * 0.12;
        diffuseColor += texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y - blurSize)) * 0.15;
        diffuseColor += texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y + blurSize)) * 0.15;
        diffuseColor += texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y + 2.0*blurSize)) * 0.12;
        diffuseColor += texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y + 3.0*blurSize)) * 0.09;
        diffuseColor += texture2D(ColorTexture, vec2(pass_TexCoord.x, pass_TexCoord.y + 4.0*blurSize)) * 0.05;

        light *= 0.16f;
        light += texture2D(LightTexture, vec2(pass_TexCoord.x, pass_TexCoord.y - 4.0*blurSize)) * 0.05;
        light += texture2D(LightTexture, vec2(pass_TexCoord.x, pass_TexCoord.y - 3.0*blurSize)) * 0.09;
        light += texture2D(LightTexture, vec2(pass_TexCoord.x, pass_TexCoord.y - 2.0*blurSize)) * 0.12;
        light += texture2D(LightTexture, vec2(pass_TexCoord.x, pass_TexCoord.y - blurSize)) * 0.15;
        light += texture2D(LightTexture, vec2(pass_TexCoord.x, pass_TexCoord.y + blurSize)) * 0.15;
        light += texture2D(LightTexture, vec2(pass_TexCoord.x, pass_TexCoord.y + 2.0*blurSize)) * 0.12;
        light += texture2D(LightTexture, vec2(pass_TexCoord.x, pass_TexCoord.y + 3.0*blurSize)) * 0.09;
        light += texture2D(LightTexture, vec2(pass_TexCoord.x, pass_TexCoord.y + 4.0*blurSize)) * 0.05;
    }*/
    out_Color = (1.0f - alpha) * vec4(diffuseColor.rgb, 1.0f) + alpha * shadow * (vec4(diffuseColor.rgb * light.rgb, 1.0f));
}

#frag_end
