#include "skybox.hpp"

#include <QImage>
#include <QMatrix4x4>

const GLfloat Skybox::skybox_vertices[] = {
    1, 1, - 1,
    - 1, - 1, - 1,
    1, - 1, - 1,
    1, 1, - 1,
    - 1, - 1, 1,
    - 1, 1, 1,
    1, - 1, 1,
    1, 1, 1,
};

const GLuint Skybox::skybox_indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 1, 0, 0, 5, 4,
    2, 6, 7, 7, 3, 2,
    4, 5, 7, 7, 6, 4,
    0, 3, 7, 7, 5, 0,
    1, 4, 2, 2, 4, 6,
};

Skybox::Skybox(const QString& px, const QString& nx, const QString& py, const QString& ny, const QString& pz, const QString& nz)
{
    Q_ASSERT(initializeOpenGLFunctions());

    // setup texture
    QImage pxi;
    QImage nxi;
    QImage pyi;
    QImage nyi;
    QImage pzi;
    QImage nzi;

    Q_ASSERT(pxi.load(px));
    Q_ASSERT(nxi.load(nx));
    Q_ASSERT(pyi.load(py));
    Q_ASSERT(nyi.load(ny));
    Q_ASSERT(pzi.load(pz));
    Q_ASSERT(nzi.load(nz));

    glGenTextures(1, &m_cubeTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTex);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, pxi.width(), pxi.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pxi.bits());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, nxi.width(), nxi.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nxi.bits());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, pyi.width(), pyi.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pyi.bits());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, nyi.width(), nyi.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nyi.bits());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, pzi.width(), pzi.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pzi.bits());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, nzi.width(), nzi.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nzi.bits());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //setup buffers
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skybox_indices), skybox_indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(0));

    // load shader
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/skybox.vert");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/skybox.frag");
    Q_ASSERT(m_program.link());

    Q_ASSERT(m_program.isLinked());
}

Skybox::~Skybox() {
    if (m_cubeTex) {
        glDeleteTextures(1, &m_cubeTex);
    }
    if (m_vbo) {
        glDeleteBuffers(1, &m_vbo);
    }
    if (m_ibo) {
        glDeleteBuffers(1, &m_ibo);
    }
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
    }
}

void Skybox::render(const QMatrix4x4& projection, QMatrix4x4 view) {
    glDepthMask(GL_FALSE);

    view.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    view.scale(10.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTex);

    glBindVertexArray(m_vao);

    Q_ASSERT(m_program.bind());
    m_program.setUniformValue(1, view);
    m_program.setUniformValue(2, projection);
    m_program.setUniformValue(3, 0);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glDepthMask(GL_TRUE);
}
