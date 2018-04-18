#include "myglwidget.hpp"

#include <iostream>
#include <QtDebug>

MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), m_cameraPos(), m_fov(45), m_angle(0),
      m_near(10.0), m_far(1000.0),
      m_rotationA(0), m_rotationB(0), m_rotationC(0)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

MyGLWidget::~MyGLWidget()
{ }

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    float xMovement = 0.0f;
    float zMovement = 0.0f;

    float increment = 0.2;

    switch (event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        zMovement += increment;
        event->accept();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        zMovement -= increment;
        event->accept();
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        xMovement -= increment;
        event->accept();
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        xMovement += increment;
        event->accept();
        break;
    default:
        QOpenGLWidget::keyPressEvent(event);
        break;
    }

    QVector3D transformation(xMovement, 0.0f, zMovement);

    m_cameraPos += transformation;
    qDebug() << "Camera position: " << m_cameraPos;
}


void MyGLWidget::setFOV(int val)
{
    m_fov = val;
    qDebug() << "FOV changed: " << m_fov;
}

void MyGLWidget::setAngle(int angle)
{
    m_angle = angle;
    qDebug() << "Angle changed: " << m_angle;
}

void MyGLWidget::setProjectionMode(bool perspective)
{
    m_perspective = perspective;
    qDebug() << "Perspective " << (perspective ? "enabled" : "disabled");
}

void MyGLWidget::setNear(double val)
{
    if (val > m_far - MAX_NEAR_FAR_DELTA)
    {
        farChanged(val + MAX_NEAR_FAR_DELTA);
    }

    m_near = val;

    qDebug() << "Near changed: " << m_near;
    nearChanged(val);
}

void MyGLWidget::setFar(double val)
{
    if (val < m_near + MAX_NEAR_FAR_DELTA)
    {
        nearChanged(val - MAX_NEAR_FAR_DELTA);
    }

    m_far = val;

    qDebug() << "Far changed: " << m_far;
    farChanged(val);
}

void MyGLWidget::setRotationA(int val)
{
    m_rotationA = val;
    qDebug() << "rotationA changed: " << m_rotationA;
}

void MyGLWidget::setRotationB(int val)
{
    m_rotationB = val;
    qDebug() << "rotationB changed: " << m_rotationB;
}

void MyGLWidget::setRotationC(int val)
{
    m_rotationC = val;
    qDebug() << "rotationC changed: " << m_rotationC;
}
