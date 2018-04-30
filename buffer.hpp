#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "vertex.hpp"

#include <QOpenGLFunctions_3_3_Core>

class buffer : public QOpenGLFunctions_3_3_Core
{
private:
    GLuint m_id;
    GLenum m_target;
public:
    buffer(GLenum target);
    ~buffer();

    void bind();
    void unbind();

    void buffer_data(const std::vector<vertex>& vertices, GLenum usage);
    void attrib_pointer(const std::vector<attribute_description> attrib_descriptions);


};

#endif // BUFFER_HPP
