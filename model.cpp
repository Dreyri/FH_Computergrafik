#include "model.hpp"

#include <QImage>
#include <QMatrix4x4>

#include <vector>

#include "vertex.hpp"

Model::Model(const QString& model_path, const QString& texture_path)
{
    initializeOpenGLFunctions();

    if (!m_loader.loadObjectFromFile(model_path)) {
        throw std::runtime_error((QString("Failed to load object \"") + model_path + "\"").toStdString());
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);

    // get our vertices and indices
    m_num_indices = m_loader.lengthOfIndexArray();
    std::vector<GLfloat> vertices(m_loader.lengthOfVBO());
    std::vector<GLuint> indices(m_num_indices);

    m_loader.genVBO(vertices.data());
    m_loader.genIndexArray(indices.data());

    // bind vao and buffers
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    // upload data to the gpu
    glBufferData(GL_ARRAY_BUFFER, sizeof(decltype(vertices)::value_type) * vertices.size(), reinterpret_cast<const GLvoid*>(vertices.data()), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices)::value_type) * indices.size(), reinterpret_cast<const GLvoid*>(indices.data()), GL_STATIC_DRAW);

    // enable vertex attributes for shaders
    for (auto& description : vertex3::vertex_description()) {
        glEnableVertexAttribArray(description.location);
        glVertexAttribPointer(description.location, description.size, description.type, description.normalized, description.stride, reinterpret_cast<const GLvoid*>(description.offset));
    }

    // unbind vao and vbos to be safe
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // check for errors
    GLenum error{glGetError()};
    if (error != GL_NO_ERROR) {
        qDebug() << "Error during model loading " << error;
    }

    // load texture
    load_texture(texture_path);

}

Model::Model(const Model& other, const QString& texture_path)
    : m_loader{other.m_loader}, m_texture{0}, m_vao{other.m_vao}, m_vbo{other.m_vbo}, m_ibo{other.m_ibo}, m_num_indices{other.m_num_indices}
{
    initializeOpenGLFunctions();
    load_texture(texture_path);
}

Model::~Model()
{
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
    }

    if (m_vbo) {
        glDeleteBuffers(1, &m_vbo);
    }

    if (m_ibo) {
        glDeleteBuffers(1, &m_ibo);
    }

    if (m_texture) {
        glDeleteTextures(1, &m_texture);
    }
}

void Model::load_texture(const QString& path) {
    QImage img;
    img.load(path);
    Q_ASSERT(!img.isNull());

    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Model::render(GLenum mode) {
    // set up render state
    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // draw indices
    glDrawElements(mode, m_num_indices, GL_UNSIGNED_INT, nullptr);

    // flush all errors
    GLenum error{glGetError()};
    if (error != GL_NO_ERROR) {
        qDebug() << "Error whilst rendering model" << error;
    }
}


