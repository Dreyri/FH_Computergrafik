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

    QVector3D m_position{0.0f, 0.0f, 0.0f};
    QVector3D m_scale{1.0f, 1.0f, 1.0f};
    QQuaternion m_rotation{};

    GLuint m_texture{0};

    GLuint m_vao{0};
    GLuint m_vbo{0};
    GLuint m_ibo{0};

    GLuint m_num_indices{0};
public:
    Model(const QString& model_path, const QString& texture_path);
    ~Model();

    void render(GLenum mode);

    void translate(const QVector3D& translation);
    void scale(const QVector3D& scale);
    void rotate(float angle, const QVector3D& axis);
    void rotate(const QQuaternion& q);

    void set_rotation(float angle, const QVector3D& axis);

    QMatrix4x4 transformation() const;
};

#endif // MODEL_HPP
