#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;

layout(location = 0) out vec3 v_color;

void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
    v_color = a_color;
}
