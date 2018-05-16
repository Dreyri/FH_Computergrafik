#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 v_tex_coord;

layout(location = 3) uniform samplerCube u_texture;

layout(location = 0) out vec4 frag_color;

void main() {
    frag_color = texture(u_texture, v_tex_coord);
}
