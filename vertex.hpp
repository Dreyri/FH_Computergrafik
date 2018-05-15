#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <QVector2D>
#include <QVector3D>
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
  QVector2D position;
  QVector3D color;
  QVector2D texcoords;

  vertex(const QVector2D& pos, const QVector3D& col, const QVector2D& tex)
      : position(pos), color(col), texcoords(tex) {}

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

    attribute_description texcoord_description = {};
    texcoord_description.location = 2;
    texcoord_description.size = sizeof(texcoords) / sizeof(GLfloat);
    texcoord_description.type = GL_FLOAT;
    texcoord_description.normalized = GL_FALSE;
    texcoord_description.stride = sizeof(vertex);
    texcoord_description.offset = offsetof(vertex, texcoords);

    return {position_description, color_description, texcoord_description};
  }
};

struct vertex3 {
    QVector3D position;
    QVector3D normal;
    QVector2D tex_coords;

    static std::vector<attribute_description> vertex_description() {
        attribute_description position_description = {};
        position_description.location = 0;
        position_description.size = sizeof(position) / sizeof(GLfloat);
        position_description.type = GL_FLOAT;
        position_description.normalized = GL_FALSE;
        position_description.stride = sizeof(vertex3);
        position_description.offset = offsetof(vertex3, position);

        attribute_description normal_description = {};
        normal_description.location = 1;
        normal_description.size = sizeof(normal) / sizeof(GLfloat);
        normal_description.type = GL_FLOAT;
        normal_description.normalized = GL_TRUE;
        normal_description.stride = sizeof(vertex3);
        normal_description.offset = offsetof(vertex3, normal);

        attribute_description tex_coord_description = {};
        tex_coord_description.location = 2;
        tex_coord_description.size = sizeof(tex_coords) / sizeof(GLfloat);
        tex_coord_description.type = GL_FLOAT;
        tex_coord_description.normalized = GL_FALSE;
        tex_coord_description.stride = sizeof(vertex3);
        tex_coord_description.offset = offsetof(vertex3, tex_coords);

        return {position_description, normal_description, tex_coord_description};
    }
};

#endif // VERTEX_HPP
