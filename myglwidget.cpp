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
    m_gimbal_b = std::make_unique<Model>(*m_gimbal_a, ":/textures/green.png");
    m_gimbal_c = std::make_unique<Model>(*m_gimbal_a, ":/textures/blue.png");

    m_sphere = std::make_unique<Model>(":/models/sphere.obj", ":/textures/sample_texture.jpg");

    // load skybox
    m_skybox = std::make_unique<Skybox>(":/textures/skybox_images/right.jpg",  // px
                                        ":/textures/skybox_images/left.jpg",   // nx
                                        ":/textures/skybox_images/top.jpg",    // py
                                        ":/textures/skybox_images/bottom.jpg", // ny
                                        ":/textures/skybox_images/back.jpg",  // pz
                                        ":/textures/skybox_images/front.jpg");  // nz

    // set up transformations
    m_a_transform.set_scale(0.8f);
    m_b_transform.set_scale(0.8f);
    m_c_transform.set_scale(10.f);

    m_a_transform.set_parent(&m_b_transform);
    m_b_transform.set_parent(&m_c_transform);

    m_sphere_transform.move({0.0f, 1.f, 0.2f});
    m_sphere_transform.scale(0.075f);

    m_sphere_rotation.set_parent(&m_b_transform);
    m_sphere_transform.set_parent(&m_sphere_rotation);

    glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    QMatrix4x4 view;

    // if centered checkbox clicked
    if (m_center) {
        view = m_a_transform.transformation_matrix().inverted();
    }
    else {
        view.translate(m_cameraPos);
        view = view.inverted();
    }

    static qint64 old_time = 0;
    auto elapsed_ms = m_timer.elapsed();
    float degree_sec = 40.0f;
    auto dt = (static_cast<float>(elapsed_ms) - static_cast<float>(old_time)) / 1000.0f;

    if (!m_auto_rotate) {
        m_a_transform.set_rotation(QQuaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, m_rotationA));
        m_b_transform.set_rotation(QQuaternion::fromAxisAndAngle({1.0f, 0.0f, 0.0f}, m_rotationB));
        m_c_transform.set_rotation(QQuaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, m_rotationC));
    }
    else {

        m_a_transform.rotate(QQuaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, dt * degree_sec));
        m_b_transform.rotate(QQuaternion::fromAxisAndAngle({1.0f, 0.0f, 0.0f}, dt * degree_sec));
        m_c_transform.rotate(QQuaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, dt * degree_sec));


    }

    m_sphere_rotation.rotate(QQuaternion::fromAxisAndAngle({0.0f, 0.0f, 1.0f}, dt * degree_sec));
    m_sphere_transform.rotate(QQuaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, dt * -10.0f * degree_sec));

    old_time = elapsed_ms;

    // draw skybox first
    m_skybox->render(m_projection, view);

    //praktikum 3
    Q_ASSERT(m_program_p3.bind());
    m_program_p3.setUniformValue(3, 0);
    m_program_p3.setUniformValue(1, view);
    m_program_p3.setUniformValue(2, m_projection);


    auto transform = m_a_transform.transformation_matrix();
    m_program_p3.setUniformValue(0, transform);
    m_gimbal_c->render(GL_TRIANGLES);

    transform = m_b_transform.transformation_matrix();
    m_program_p3.setUniformValue(0, transform);
    m_gimbal_b->render(GL_TRIANGLES);

    transform = m_c_transform.transformation_matrix();
    m_program_p3.setUniformValue(0, transform);
    m_gimbal_a->render(GL_TRIANGLES);

    //transform = sphere_transform;

    transform = m_sphere_transform.transformation_matrix();
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

    if (m_perspective) {

        projection.perspective(m_fov, aspect_ratio, m_near, m_far);
        m_projection = projection;
        qDebug() << "using perspective";
    }
    else {
        float win_width = static_cast<float>(width()) * 0.1f;
        float win_height = static_cast<float>(height()) * 0.1f;

        projection.ortho(-win_width*0.5f, win_width*0.5f, -win_height*0.5f, win_height*0.5f, m_near, m_far);
        m_projection = projection;
        qDebug() << "using orthogonal";
    }
    qDebug() << "projection matrix is now" << m_projection;

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
        zMovement -= increment;
        event->accept();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        zMovement += increment;
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
    setupProjection();
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
