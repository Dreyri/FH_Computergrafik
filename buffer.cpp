#include "buffer.hpp"
#include "vertex.hpp"

#include <type_traits>

buffer::buffer(GLenum target)
    : m_target(target)
{
    glGenBuffers(1, &m_id);
}

buffer::~buffer()
{
    glDeleteBuffers(1, &m_id);
}

void buffer::bind()
{
    glBindBuffer(m_target, m_id);
}

void buffer::unbind()
{
    glBindBuffer(m_target, 0);
}

void buffer::buffer_data(const std::vector<vertex>& vertices, GLenum usage)
{
    glBufferData(m_target, sizeof(vertices[0]) * vertices.size(), vertices.data(), usage);
}

void buffer::attrib_pointer(const std::vector<attribute_description> attrib_descriptions)
{
    for (size_t i = 0; i < attrib_descriptions.size(); ++i)
    {
        auto& curr_description = attrib_descriptions[i];
        glVertexAttribPointer(0, curr_description.size, curr_description.type, curr_description.normalized, curr_description.stride, curr_description.offset);
    }
}
