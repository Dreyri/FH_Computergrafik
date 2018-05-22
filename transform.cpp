#include "transform.hpp"


transform::transform(const QVector3D& translation, const QVector3D& scale, const QQuaternion& rotation)
    : m_parent{nullptr}, m_translation{translation}, m_scale{scale}, m_rotation{rotation}
{}

transform* transform::parent() {
    return m_parent;
}

void transform::set_parent(transform* parent) {
    m_parent = parent;
}

const QVector3D& transform::translation() const {
    return m_translation;
}
const QVector3D& transform::scale() const {
    return m_scale;
}
const QQuaternion& transform::rotation() const {
    return m_rotation;
}

void transform::move(const QVector3D translation) {
    m_translation.setX(m_translation.x() + translation.x());
    m_translation.setY(m_translation.y() + translation.y());
    m_translation.setZ(m_translation.z() + translation.z());
}
void transform::scale(const QVector3D scale) {
    m_scale.setX(m_scale.x() * scale.x());
    m_scale.setY(m_scale.y() * scale.y());
    m_scale.setZ(m_scale.z() * scale.z());
}
void transform::scale(float scale) {
    m_scale.setX(m_scale.x() * scale);
    m_scale.setY(m_scale.y() * scale);
    m_scale.setZ(m_scale.z() * scale);
}
void transform::rotate(const QQuaternion& rotation) {
    m_rotation *= rotation;
}

void transform::set_translation(const QVector3D& translation) {
    m_translation = translation;
}
void transform::set_scale(const QVector3D& scale) {
    m_scale = scale;
}
void transform::set_scale(float scale) {
    m_scale = {scale, scale, scale};
}
void transform::set_rotation(const QQuaternion& rotation) {
    m_rotation = rotation;
}

QMatrix4x4 transform::transformation_matrix() const {
    QMatrix4x4 transformation;

    transformation.scale(m_scale);
    transformation.translate(m_translation);
    transformation.rotate(m_rotation);

    if (m_parent) {
        return m_parent->transformation_matrix() * transformation;
    }

    return transformation;
}
