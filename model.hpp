#ifndef MODEL_HPP
#define MODEL_HPP

#include <QString>
#include <QOpenGLFunctions_3_3_Core>

#include "modelloader.h"

class Model : public QOpenGLFunctions_3_3_Core
{
private:
    ModelLoader m_loader{};

    GLuint m_vao{0};
    GLuint m_vbo{0};
    GLuint m_ibo{0};

    GLuint m_num_indices{0};
public:
    Model(const QString& resource_path);
    ~Model();

    void render(GLenum mode);
};

#endif // MODEL_HPP
