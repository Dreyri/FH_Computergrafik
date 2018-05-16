#include "myglwidget.hpp"

#include "vertex.hpp"
#include <QtDebug>
#include <QElapsedTimer>

MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), m_fov(45), m_angle(0),
      m_near(1.0), m_far(1000.0),
      m_rotationA(0), m_rotationB(0), m_rotationC(0)
{
    m_timer.start();
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setupProjection();
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
    [[maybe_unused]] bool success = initializeOpenGLFunctions();

    // enable face culling
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // praktikum 3
    // load shader
    m_program_p3.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/praktikum3.vert");
    m_program_p3.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/praktikum3.frag");
    Q_ASSERT(m_program_p3.link());

    // load model
    m_gimbal_a = std::make_unique<Model>(":/models/gimbal.obj", ":/textures/red.png");
    m_gimbal_b = std::make_unique<Model>(":/models/gimbal.obj", ":/textures/green.png");
    m_gimbal_c = std::make_unique<Model>(":/models/gimbal.obj", ":/textures/blue.png");

    m_sphere = std::make_unique<Model>(":/models/sphere.obj", ":/textures/sample_texture.jpg");

    // load skybox
    m_skybox = std::make_unique<Skybox>(":/textures/skybox_images/right.jpg",  // px
                                        ":/textures/skybox_images/left.jpg",   // nx
                                        ":/textures/skybox_images/top.jpg",    // py
                                        ":/textures/skybox_images/bottom.jpg", // ny
                                        ":/textures/skybox_images/back.jpg",  // pz
                                        ":/textures/skybox_images/front.jpg");  // nz

    // set up transformations
    m_gimbal_a->scale({3.0f, 3.0f, 3.0f});
    m_gimbal_b->scale({9.0f, 9.0f, 9.0f});
    m_gimbal_c->scale({15.0f, 15.0f, 15.0f});

    glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    QMatrix4x4 view;

    // if centered checkbox clicked
    if (m_center) {
        view = m_gimbal_a->transformation().inverted();
    }
    else {
        view.translate(m_cameraPos);
        //view = view.inverted();
    }

    QVector3D axis{0.0f, 1.0f, 0.0f};
    QMatrix4x4 sphere_transform;

    auto elapsed_ms = m_timer.elapsed();

    float sphere_rotation = static_cast<float>(elapsed_ms) / 20.0f;
    m_sphere->rotate(sphere_rotation, {1.0f, 0.0f, 0.0f});


    // if auto rotation checkbox clicked
    // this is handled a bit ugly but this praktikum is almost done
    // so maybe clean up for the next one
    if (m_auto_rotate) {
        auto ms = m_timer.elapsed();

        float rotationA = static_cast<double>(ms) / 100.0; // 10 degrees/s
        float rotationB = -(static_cast<double>(ms) / 50.0); // -20 degrees/s
        float rotationC = static_cast<double>(ms) / 33.0; // 30 degrees/s

        m_gimbal_a->set_rotation(rotationA, axis);
        m_gimbal_b->set_rotation(rotationB, axis);
        m_gimbal_c->set_rotation(rotationC, axis);

        sphere_transform.rotate(rotationB, QVector3D(0.0f, 1.0f, 0.0f));
        sphere_transform.rotate(m_angle, QVector3D(0.0f, 0.0f, 1.0f));
        sphere_transform.translate(0.0f, 11.0f, 0.0f);
    }
    else {
        m_gimbal_a->set_rotation(m_rotationA, axis);
        m_gimbal_b->set_rotation(m_rotationB, axis);
        m_gimbal_c->set_rotation(m_rotationC, axis);

        sphere_transform.rotate(m_rotationB, QVector3D(0.0f, 1.0f, 0.0f));
        sphere_transform.rotate(m_angle, QVector3D(0.0f, 0.0f, 1.0f));
        sphere_transform.translate(0.0f, 11.0f, 0.0f);
    }

    // draw skybox first
    m_skybox->render(m_projection, view);

    //praktikum 3
    Q_ASSERT(m_program_p3.bind());
    m_program_p3.setUniformValue(3, 0);
    m_program_p3.setUniformValue(1, view);
    m_program_p3.setUniformValue(2, m_projection);

    auto transform = m_gimbal_a->transformation();
    m_program_p3.setUniformValue(0, transform);
    m_gimbal_a->render(GL_TRIANGLES);

    transform = m_gimbal_b->transformation();
    m_program_p3.setUniformValue(0, transform);
    m_gimbal_b->render(GL_TRIANGLES);

    transform = m_gimbal_c->transformation();
    m_program_p3.setUniformValue(0, transform);
    m_gimbal_c->render(GL_TRIANGLES);

    transform = sphere_transform;
    transform *= m_sphere->transformation();
    //transform = m_sphere->transformation();
    //transform *= sphere_transform;
    m_program_p3.setUniformValue(0, transform);
    m_sphere->render(GL_TRIANGLES);

    update();
}

void MyGLWidget::resizeGL([[maybe_unused]] int w, [[maybe_unused]] int h)
{
    setupProjection();
}

void MyGLWidget::setupProjection() {
    float aspect_ratio = static_cast<float>(width()) / static_cast<float>(height());
    QMatrix4x4 projection;

    projection.perspective(m_fov, aspect_ratio, m_near, m_far);
    m_projection = projection;
    qDebug() << "projection matrix is now " << m_projection;
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

void MyGLWidget::setCenter(bool centerCamera) {
    m_center = centerCamera;
    qDebug() <<"Camera centered changed:" << m_center;
}

void MyGLWidget::setAutoRotate(bool rotate) {
    m_auto_rotate = rotate;
    qDebug() << "Auto rotate" << m_auto_rotate;
}

void MyGLWidget::setFOV(int val)
{
    m_fov = val;
    qDebug() << "FOV changed: " << m_fov;
    setupProjection();
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
    setupProjection();
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
    setupProjection();
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
