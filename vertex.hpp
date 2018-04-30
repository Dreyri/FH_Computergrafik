#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <QVector2D>
#include <vector>
#include <QOpenGLFunctions_3_3_Core>

struct attribute_description
{
    GLenum type;
    size_t size;
    bool normalized;
    size_t stride;
    size_t offset;
};

struct vertex
{
  QVector2D pos;

  static std::vector<attribute_description> vertex_description()
  {
    attribute_description position_description = {};
    position_description.size = 2;
    position_description.type = GL_FLOAT;
    position_description.normalized = false;
    position_description.stride = sizeof(vertex);
    position_description.offset = offsetof(vertex, pos);

    return {position_description};
  }
};

#endif // VERTEX_HPP
