#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_tex_coord;

layout(location = 0) uniform mat4 u_model;
layout(location = 1) uniform mat4 u_view;
layout(location = 2) uniform mat4 u_projection;

layout(location = 0) out vec3 v_color;
layout(location = 1) out vec2 v_tex_coord;

void main() {
    v_tex_coord = a_position.xy;
    v_color = a_color;
    vec4 pos = vec4(a_position, 1.0);
    gl_Position = u_projection * u_view * u_model * pos;

}
