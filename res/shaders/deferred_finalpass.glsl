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

out vec4 out_Color;

void main(void)
{
    vec3 diffuseColor = texture2D(ColorTexture, pass_TexCoord).rgb;
    vec4 light = texture2D(LightTexture, pass_TexCoord);

    vec3 diffuseLight = light.rgb;
    float specularLight = light.a;

    out_Color = vec4(diffuseColor * (diffuseLight + specularLight*0.2), 1.0f);
}

#frag_end
