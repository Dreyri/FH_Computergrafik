#ifndef MYGLWIDGET_HPP
#define MYGLWIDGET_HPP

#include <QWidget>
#include <QOpenGLWidget>

class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget* parent = nullptr);

public slots:
    void setFOV(int val);
    void setAngle(int angle);
    void setProjectionMode();
    void setNear(double val);
    void setFar(double val);
    setRotationA(int val);
    setRotationB(int val);
    setRotationC(int val);
};

#endif // MYGLWIDGET_HPP
