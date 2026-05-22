
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsView>
#include <chrono>

MainWindow::MainWindow()
{
    QWidget* central = new QWidget;
    setCentralWidget(central);

    resize(1200, 700);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    scene = new QGraphicsScene(this);

    view = new QGraphicsView(scene);
    view->setBackgroundBrush(Qt::black);

    mainLayout->addWidget(view, 1);

    QVBoxLayout* right = new QVBoxLayout;

    mainLayout->addLayout(right);

    QPushButton* loadBtn = new QPushButton("Load XYZ");
    QPushButton* genBtn = new QPushButton("Generate");

    right->addWidget(loadBtn);
    right->addWidget(genBtn);

    listWidget = new QListWidget;

    right->addWidget(listWidget);

    QPushButton* tBtn = new QPushButton("+ Translate");
    QPushButton* rxBtn = new QPushButton("+ Rotate X");
    QPushButton* ryBtn = new QPushButton("+ Rotate Y");
    QPushButton* rzBtn = new QPushButton("+ Rotate Z");
    QPushButton* sBtn = new QPushButton("+ Scale");

    right->addWidget(tBtn);
    right->addWidget(rxBtn);
    right->addWidget(ryBtn);
    right->addWidget(rzBtn);
    right->addWidget(sBtn);

    QPushButton* removeBtn = new QPushButton("Remove");
    QPushButton* clearBtn = new QPushButton("Clear");

    right->addWidget(removeBtn);
    right->addWidget(clearBtn);

    QPushButton* seqBtn = new QPushButton("Apply Sequential");
    QPushButton* parBtn = new QPushButton("Apply Parallel");

    right->addWidget(seqBtn);
    right->addWidget(parBtn);

    QPushButton* downBtn = new QPushButton("Downsample");

    right->addWidget(downBtn);

    stepSlider = new QSlider(Qt::Horizontal);

    stepSlider->setMinimum(0);

    right->addWidget(stepSlider);

    matrixEdit = new QTextEdit;

    right->addWidget(matrixEdit);

    statsLabel = new QLabel;

    right->addWidget(statsLabel);

    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadCloud);
    connect(genBtn, &QPushButton::clicked, this, &MainWindow::generateCloud);

    connect(tBtn, &QPushButton::clicked, this, &MainWindow::addTranslate);
    connect(rxBtn, &QPushButton::clicked, this, &MainWindow::addRotateX);
    connect(ryBtn, &QPushButton::clicked, this, &MainWindow::addRotateY);
    connect(rzBtn, &QPushButton::clicked, this, &MainWindow::addRotateZ);
    connect(sBtn, &QPushButton::clicked, this, &MainWindow::addScale);

    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::removeSelected);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearAll);

    connect(seqBtn, &QPushButton::clicked, this, &MainWindow::applySequential);
    connect(parBtn, &QPushButton::clicked, this, &MainWindow::applyParallel);

    connect(downBtn, &QPushButton::clicked, this, &MainWindow::downsample);

    connect(stepSlider, &QSlider::valueChanged, this, &MainWindow::sliderChanged);
}

void MainWindow::loadCloud()
{
    QString file = QFileDialog::getOpenFileName(this, "Open", "", "*.xyz");

    if (cloud.loadXYZ(file))
    {
        originalCloud = cloud;
        redrawCloud(cloud);
    }
}

void MainWindow::generateCloud()
{
    cloud.generateTestCloud(3000);

    originalCloud = cloud;

    redrawCloud(cloud);
}

void MainWindow::redrawCloud(const PointCloud& cloud)
{
    view->setUpdatesEnabled(false);

    scene->clear();

    for (const auto& p : cloud.points)
    {
        scene->addEllipse(
            p.x,
            -p.y,
            2,
            2,
            Qt::NoPen,
            QBrush(Qt::white));
    }

    scene->setSceneRect(scene->itemsBoundingRect());

    view->fitInView(
        scene->sceneRect(),
        Qt::KeepAspectRatio);

    view->setUpdatesEnabled(true);

    view->viewport()->update();
}

void MainWindow::addTranslate()
{
    double dx = QInputDialog::getDouble(this, "", "dx");
    double dy = QInputDialog::getDouble(this, "", "dy");
    double dz = QInputDialog::getDouble(this, "", "dz");

    TransformOperation op;
    op.type = TransformType::Translate;
    op.a = dx;
    op.b = dy;
    op.c = dz;

    transforms.push_back(op);

    listWidget->addItem(op.toString());

    stepSlider->setMaximum(transforms.size());
}

void MainWindow::addRotateX()
{
    double a = QInputDialog::getDouble(this, "", "angle");

    TransformOperation op;
    op.type = TransformType::RotateX;
    op.a = a;

    transforms.push_back(op);

    listWidget->addItem(op.toString());

    stepSlider->setMaximum(transforms.size());
}

void MainWindow::addRotateY()
{
    double a = QInputDialog::getDouble(this, "", "angle");

    TransformOperation op;
    op.type = TransformType::RotateY;
    op.a = a;

    transforms.push_back(op);

    listWidget->addItem(op.toString());

    stepSlider->setMaximum(transforms.size());
}

void MainWindow::addRotateZ()
{
    double a = QInputDialog::getDouble(this, "", "angle");

    TransformOperation op;
    op.type = TransformType::RotateZ;
    op.a = a;

    transforms.push_back(op);

    listWidget->addItem(op.toString());

    stepSlider->setMaximum(transforms.size());
}

void MainWindow::addScale()
{
    double s = QInputDialog::getDouble(this, "", "scale");

    TransformOperation op;
    op.type = TransformType::Scale;
    op.a = s;

    transforms.push_back(op);

    listWidget->addItem(op.toString());

    stepSlider->setMaximum(transforms.size());
}

void MainWindow::removeSelected()
{
    int row = listWidget->currentRow();

    if (row >= 0)
    {
        transforms.erase(transforms.begin() + row);

        delete listWidget->takeItem(row);
    }
}

void MainWindow::clearAll()
{
    transforms.clear();

    listWidget->clear();
}

Matrix4x4 MainWindow::composeMatrix(int count)
{
    Matrix4x4 total = Matrix4x4::identity();

    for (int i = 0; i < count; ++i)
        total = total * transforms[i].toMatrix();

    return total;
}

void MainWindow::applyAll(bool parallel)
{
    cloud = originalCloud;

    Matrix4x4 total = composeMatrix(transforms.size());

    auto start = std::chrono::high_resolution_clock::now();

    if (parallel)
        cloud.applyMatrixParallel(total);
    else
        cloud.applyMatrixSequential(total);

    auto end = std::chrono::high_resolution_clock::now();

    double ms =
        std::chrono::duration<double, std::milli>(end - start).count();

    redrawCloud(cloud);

    QString text;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            text += QString::number(total.m[i][j]) + " ";
        }

        text += "\n";
    }

    matrixEdit->setText(text);

    statsLabel->setText(
        QString("Points: %1\nTime: %2 ms")
            .arg(cloud.size())
            .arg(ms));
}

void MainWindow::applySequential()
{
    applyAll(false);
}

void MainWindow::applyParallel()
{
    applyAll(true);
}

void MainWindow::downsample()
{
    cloud.downsample(2);

    redrawCloud(cloud);
}

void MainWindow::sliderChanged(int value)
{
    PointCloud temp = originalCloud;

    Matrix4x4 total = composeMatrix(value);

    temp.applyMatrixSequential(total);

    redrawCloud(temp);
}
