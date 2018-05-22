#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>


class transform
{
private:
    transform* m_parent{nullptr};

    QVector3D m_translation{0.0f, 0.0f, 0.0f};
    QVector3D m_scale{1.0f, 1.0f, 1.0f};
    QQuaternion m_rotation{};
public:
    transform() = default;
    transform(const QVector3D& translation, const QVector3D& scale, const QQuaternion& rotation);

    transform(const transform&) = default;
    transform& operator=(const transform&) = default;

    transform* parent();
    void set_parent(transform* parent);

    const QVector3D& translation() const;
    const QVector3D& scale() const;
    const QQuaternion& rotation() const;

    void move(const QVector3D translation);
    void scale(const QVector3D scale);
    void scale(float scale);
    void rotate(const QQuaternion& rotation);

    void set_translation(const QVector3D& translation);
    void set_scale(const QVector3D& scale);
    void set_scale(float scale);
    void set_rotation(const QQuaternion& rotation);

    QMatrix4x4 transformation_matrix() const;
};

#endif // TRANSFORM_HPP
