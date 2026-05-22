
#pragma once

#include <vector>
#include <QString>

struct Point3D
{
    double x;
    double y;
    double z;
};

class Matrix4x4;

class PointCloud
{
public:
    std::vector<Point3D> points;

    bool loadXYZ(const QString& filename);

    void generateTestCloud(int n);

    void applyMatrixSequential(const Matrix4x4& mat);

    void applyMatrixParallel(const Matrix4x4& mat);

    void downsample(int step);

    size_t size() const;
};
