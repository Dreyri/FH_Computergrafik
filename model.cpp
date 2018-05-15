#include "model.hpp"

#include <vector>

#include "vertex.hpp"

Model::Model(const QString& resource_path)
{
    initializeOpenGLFunctions();

    if (!m_loader.loadObjectFromFile(resource_path)) {
        throw std::runtime_error((QString("Failed to load object \"") + resource_path + "\"").toStdString());
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
}

void Model::render(GLenum mode) {
    glBindVertexArray(m_vao);
    glDrawElements(mode, m_num_indices, GL_UNSIGNED_INT, nullptr);

    // flush all errors
    GLenum error{glGetError()};
    if (error != GL_NO_ERROR) {
        qDebug() << "Error whilst rendering model " << error;
    }
}
