#ifndef MYGLWIDGET_HPP
#define MYGLWIDGET_HPP

#include <QWidget>
#include <QOpenGLWidget>

class MyGLWidget : public QOpenGLWidget
{
private:
    int m_fov;
    int m_angle;

    bool m_perspective;

    double m_near;
    double m_far;

    int m_rotationA;
    int m_rotationB;
    int m_rotationC;
public:
    MyGLWidget(QWidget* parent = nullptr);
    virtual ~MyGLWidget();

public slots:
    void setFOV(int val);
    void setAngle(int angle);
    void setProjectionMode(bool perspective);
    void setNear(double val);
    void setFar(double val);
    void setRotationA(int val);
    void setRotationB(int val);
    void setRotationC(int val);
};

#endif // MYGLWIDGET_HPP
