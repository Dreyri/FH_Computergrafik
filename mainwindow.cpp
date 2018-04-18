#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QSlider>

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
    connect(ui->openGLWidget, &MyGLWidget::nearChanged, ui->dsbNear, &QDoubleSpinBox::setValue);

    connect(ui->dsbFar, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setFar);
    connect(ui->openGLWidget, &MyGLWidget::farChanged, ui->dsbFar, &QDoubleSpinBox::setValue);

    connect(ui->hsRotationA, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationA);
    connect(ui->hsRotationB, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationB);
    connect(ui->hsRotationC, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationC);

    connect(ui->rbReset, &QPushButton::pressed, this, &MainWindow::reset);

    reset();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    ui->vsAngle->setValue(70);
    ui->vsFOV->setValue(70);

    ui->rbPerspective->setChecked(true);

    ui->dsbNear->setValue(0.0);
    ui->dsbFar->setValue(1000.0);

    ui->hsRotationA->setValue(0);
    ui->hsRotationB->setValue(0);
    ui->hsRotationC->setValue(0);
}
