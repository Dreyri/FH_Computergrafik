#ifndef MYGLWIDGET_HPP
#define MYGLWIDGET_HPP

#include <QWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QKeyEvent>
#include <QOpenGLShader>

class MyGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
private:
    static constexpr double MAX_NEAR_FAR_DELTA = 2.0;

    QVector3D m_cameraPos;
    QOpenGLShaderProgram m_shaderprog;
    QOpenGLShaderProgram m_shaderprog2;

    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ibo;

    GLuint m_texture;

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

    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

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
