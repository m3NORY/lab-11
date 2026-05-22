
#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QListWidget>
#include <QSlider>
#include <QTextEdit>
#include <QLabel>
#include <QGraphicsView>
#include "../core/pointcloud.h"
#include "../core/transform.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    PointCloud cloud;
    PointCloud originalCloud;

    std::vector<TransformOperation> transforms;

    QGraphicsView* view;
    QGraphicsScene* scene;

    QListWidget* listWidget;

    QTextEdit* matrixEdit;
    QLabel* statsLabel;

    QSlider* stepSlider;

    void redrawCloud(const PointCloud& cloud);

    void applyAll(bool parallel);

    Matrix4x4 composeMatrix(int count);

private slots:
    void loadCloud();

    void generateCloud();

    void addTranslate();

    void addRotateX();

    void addRotateY();

    void addRotateZ();

    void addScale();

    void removeSelected();

    void clearAll();

    void applySequential();

    void applyParallel();

    void downsample();

    void sliderChanged(int value);
};
