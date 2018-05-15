#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 v_color;
layout(location = 1) in vec2 v_tex_coords;
layout(location = 2) uniform float u_alpha;
layout(location = 3) uniform sampler2D u_tex;

layout(location = 4) uniform float u_texcoord_offset;

layout(location = 0) out vec4 frag_color;

void main()
{
    vec2 adjusted_tex_coords = vec2(v_tex_coords.x + u_texcoord_offset, v_tex_coords.y + u_texcoord_offset);
    frag_color = vec4(texture(u_tex, adjusted_tex_coords).rgb, u_alpha);
}
