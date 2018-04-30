#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 a_position;

void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
}
