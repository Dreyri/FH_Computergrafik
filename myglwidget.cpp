#include "myglwidget.hpp"

#include "vertex.hpp"
#include <glm/glm.hpp>
#include <QtDebug>

MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), m_cameraPos(), m_shaderprog(), m_fov(45), m_angle(0),
      m_near(10.0), m_far(1000.0),
      m_rotationA(0), m_rotationB(0), m_rotationC(0)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

MyGLWidget::~MyGLWidget()
{
    GLuint buffers[] = {m_vbo, m_ibo};
    glDeleteTextures(1, &m_texture);
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &m_vao);
}

void MyGLWidget::initializeGL()
{
    bool success = initializeOpenGLFunctions();

    QImage img;
    img.load(":/textures/sample_texture.jpg");
    Q_ASSERT(!img.isNull());

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //load texture

    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    //generate buffers

    vertex vertices[] = {vertex(QVector2D(-0.5f, -0.5f), QVector3D(1.0f, 0.f, 0.f), QVector2D(0.f, 1.f)),
                         vertex(QVector2D(0.5f, -0.5f), QVector3D(0.f, 1.f, 0.f), QVector2D(1.f, 1.f)),
                         vertex(QVector2D(0.5f, 0.5f), QVector3D(0.f, 0.f, 1.f), QVector2D(0.5f, 0.f)),
                         vertex(QVector2D(-0.5f, 0.5f), QVector3D(0.f, 0.f, 0.f), QVector2D(0.f, 0.f))};

    GLuint indices[] = {0, 1, 2,   0, 2, 3};

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //bind buffer descriptors

    for (const auto& description : vertex::vertex_description())
    {
        glEnableVertexAttribArray(description.location);
        glVertexAttribPointer(description.location, description.size, description.type, description.normalized, description.stride, reinterpret_cast<GLvoid*>(description.offset));
    }

    glBindVertexArray(0);

    //create shaders

    m_shaderprog.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/sample.vert");
    m_shaderprog.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/sample.frag");
    Q_ASSERT(m_shaderprog.link());

    m_shaderprog2.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/sample.vert");
    m_shaderprog2.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/color.frag");
    Q_ASSERT(m_shaderprog2.link());
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_shaderprog.bind();
    m_shaderprog.setUniformValue(2, 1.f);
    m_shaderprog.setUniformValue(3, 0); //set texture to GL_TEXTURE0
    m_shaderprog.setUniformValue(4, (static_cast<float>(m_rotationA) / 180.f) - 1.f);

    m_shaderprog2.bind();
    m_shaderprog2.setUniformValue(2, 1.f);

    //bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    m_shaderprog2.bind();

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    m_shaderprog.bind();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(sizeof(GLuint) * 3));

    update();

}

void MyGLWidget::resizeGL(int w, int h)
{

}

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
