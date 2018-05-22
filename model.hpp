#ifndef MODEL_HPP
#define MODEL_HPP

#include <QString>
#include <QOpenGLFunctions_3_3_Core>
#include <QQuaternion>

#include "modelloader.h"

class Model : public QOpenGLFunctions_3_3_Core
{
private:
    ModelLoader m_loader{};

    GLuint m_texture{0};

    GLuint m_vao{0};
    GLuint m_vbo{0};
    GLuint m_ibo{0};

    GLuint m_num_indices{0};
public:
    Model(const QString& model_path, const QString& texture_path);
    Model(const Model& other, const QString& texture_path);
    ~Model();

    void load_texture(const QString& path);

    void render(GLenum mode);
};

#endif // MODEL_HPP
