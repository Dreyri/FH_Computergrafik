#ifndef VERTEX_ARRAY_OBJECT_HPP
#define VERTEX_ARRAY_OBJECT_HPP

#include <QOpenGLFunctions_3_3_Core>


class vertex_array_object : public QOpenGLFunctions_3_3_Core
{
private:
    GLuint m_id;
public:
    vertex_array_object();
    ~vertex_array_object();

    void bind() const;
    void unbind() const;
};

typedef vertex_array_object vao;

#endif // VERTEX_ARRAY_OBJECT_HPP
