#include "vertex_array_object.hpp"

vertex_array_object::vertex_array_object()
{
    glGenVertexArrays(1, &m_id);
}

vertex_array_object::~vertex_array_object()
{
    glDeleteVertexArrays(1, &m_id);
}


void vertex_array_object::bind() const
{
    glBindVertexArray(m_id);
}

void vertex_array_object::unbind() const
{
    glBindVertexArray(0);
}
