#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

class Skybox : public QOpenGLFunctions_3_3_Core
{
private:
    static const GLfloat skybox_vertices[];
    static const GLuint skybox_indices[];

    GLuint m_cubeTex{0};

    GLuint m_vao{0};
    GLuint m_vbo{0};
    GLuint m_ibo{0};

    QOpenGLShaderProgram m_program{};
public:
    Skybox(const QString& px, const QString& nx, const QString& py, const QString& ny, const QString& pz, const QString& nz);
    ~Skybox();

    void render(const QMatrix4x4& projection, QMatrix4x4 view);

};

#endif // SKYBOX_HPP
