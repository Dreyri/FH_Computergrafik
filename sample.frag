#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 v_color;
layout(location = 1) uniform float u_alpha;

layout(location = 0) out vec4 frag_color;

void main()
{
    frag_color = vec4(v_color, u_alpha);
}
