#vert_start

#version 330 core

#define ATTR_POSITION   0

layout(location = ATTR_POSITION) in vec3 in_Position;

uniform mat4 in_MVP;

void main(void)
{
    gl_Position = in_MVP * vec4(in_Position, 1.0);
}

#vert_end

#frag_start

#version 330 core

void main(void)
{
}

#frag_end
