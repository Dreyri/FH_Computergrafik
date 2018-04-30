#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <QVector2D>
#include <vector>
#include <QOpenGLFunctions_3_3_Core>

struct attribute_description
{
    int location;
    GLenum type;
    size_t size;
    int normalized;
    size_t stride;
    size_t offset;
};

struct vertex
{
  GLfloat position[2];
  GLfloat color[3];

  static std::vector<attribute_description> vertex_description()
  {
    attribute_description position_description = {};
    position_description.location = 0;
    position_description.size = sizeof(position) / sizeof(GLfloat);
    position_description.type = GL_FLOAT;
    position_description.normalized = GL_FALSE;
    position_description.stride = sizeof(vertex);
    position_description.offset = offsetof(vertex, position);

    attribute_description color_description = {};
    color_description.location = 1;
    color_description.size = sizeof(color) / sizeof(GLfloat);
    color_description.type = GL_FLOAT;
    color_description.normalized = GL_FALSE;
    color_description.stride = sizeof(vertex);
    color_description.offset = offsetof(vertex, color);

    return {position_description, color_description};
  }
};

#endif // VERTEX_HPP
