#include "myglwidget.hpp"

#include <iostream>

MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), m_fov(45), m_angle(0),
      m_near(10.0), m_far(1000.0),
      m_rotationA(0), m_rotationB(0), m_rotationC(0)
{

}

MyGLWidget::~MyGLWidget()
{ }

void MyGLWidget::setFOV(int val)
{
    m_fov = val;
    std::cout << "FOV changed: " << m_fov << std::endl;
}

void MyGLWidget::setAngle(int angle)
{
    m_angle = angle;
    std::cout << "Angle changed: " << m_angle << std::endl;
}

void MyGLWidget::setProjectionMode(bool perspective)
{
    m_perspective = perspective;
    std::cout << "Perspective " << (perspective ? "enabled" : "disabled") << std::endl;
}

void MyGLWidget::setNear(double val)
{
    m_near = val;
    std::cout << "Near changed: " << m_near << std::endl;
}

void MyGLWidget::setFar(double val)
{
    m_far = val;
    std::cout << "Far changed: " << m_far << std::endl;
}

void MyGLWidget::setRotationA(int val)
{
    m_rotationA = val;
    std::cout << "rotationA changed: " << m_rotationA << std::endl;
}

void MyGLWidget::setRotationB(int val)
{
    m_rotationB = val;
    std::cout << "rotationB changed: " << m_rotationB << std::endl;
}

void MyGLWidget::setRotationC(int val)
{
    m_rotationC = val;
    std::cout << "rotationC changed: " << m_rotationC << std::endl;
}
