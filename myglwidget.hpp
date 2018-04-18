#ifndef MYGLWIDGET_HPP
#define MYGLWIDGET_HPP

#include <QWidget>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QKeyEvent>

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT
private:
    static constexpr double MAX_NEAR_FAR_DELTA = 2.0;

    QVector3D m_cameraPos;

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

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;

public slots:
    void setFOV(int val);
    void setAngle(int angle);
    void setProjectionMode(bool perspective);
    void setNear(double val);
    void setFar(double val);
    void setRotationA(int val);
    void setRotationB(int val);
    void setRotationC(int val);

signals:
    void nearChanged(double);
    void farChanged(double);

};

#endif // MYGLWIDGET_HPP
