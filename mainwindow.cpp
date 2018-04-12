#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->vsAngle, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setAngle);
    connect(ui->vsFOV, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setFOV);

    connect(ui->rbPerspective, &QRadioButton::clicked, ui->openGLWidget, &MyGLWidget::setProjectionMode);
    connect(ui->rbOrthogonal, &QRadioButton::clicked, [&](bool clicked) { ui->openGLWidget->setProjectionMode(!clicked); });

    connect(ui->dsbNear, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setNear);
    connect(ui->dsbFar, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setFar);

    connect(ui->hsRotationA, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationA);
    connect(ui->hsRotationB, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationB);
    connect(ui->hsRotationC, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationC);
}

MainWindow::~MainWindow()
{
    delete ui;
}
