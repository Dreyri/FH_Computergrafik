#ifndef MYGLWIDGET_HPP
#define MYGLWIDGET_HPP

#include <QWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QKeyEvent>
#include <QOpenGLShader>
#include <QElapsedTimer>

#include <memory>

#include "model.hpp"
#include "skybox.hpp"

class MyGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
private:
    static constexpr double MAX_NEAR_FAR_DELTA = 2.0;

    QMatrix4x4 m_projection;
    QMatrix4x4 m_view = QMatrix4x4(0.f, 0.f,0.f, 0.f,0.f, 0.f,0.f, 0.f,0.f, 0.f,0.f, 0.f,0.f, 0.f,0.f, 0.f);

    std::unique_ptr<Model> m_gimbal_a;
    std::unique_ptr<Model> m_gimbal_b;
    std::unique_ptr<Model> m_gimbal_c;

    std::unique_ptr<Skybox> m_skybox;

    std::unique_ptr<Model> m_sphere;

    QVector3D m_cameraPos{0.0f, 0.0f, -20.0f};

    QOpenGLShaderProgram m_program_p3;

    QElapsedTimer m_timer{};

    int m_fov;
    int m_angle;

    bool m_perspective;

    bool m_center{false};
    bool m_auto_rotate{false};

    double m_near;
    double m_far;

    int m_rotationA;
    int m_rotationB;
    int m_rotationC;
public:
    MyGLWidget(QWidget* parent = nullptr);
    virtual ~MyGLWidget();

    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    void setupProjection();

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;

public slots:
    void setCenter(bool centerCamera);
    void setAutoRotate(bool rotate);
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
