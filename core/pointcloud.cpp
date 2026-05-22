
#include "pointcloud.h"
#include "matrix4x4.h"

#include <QFile>
#include <QTextStream>

#include <random>
#include <execution>
#include <algorithm>

bool PointCloud::loadXYZ(const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream in(&file);

    points.clear();

    while (!in.atEnd())
    {
        Point3D p;
        in >> p.x >> p.y >> p.z;

        if (in.status() == QTextStream::Ok)
            points.push_back(p);
    }

    return true;
}

void PointCloud::generateTestCloud(int n)
{
    points.clear();

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dist(-200, 200);

    for (int i = 0; i < n; ++i)
    {
        points.push_back({
            dist(gen),
            dist(gen),
            dist(gen)
        });
    }
}

void PointCloud::applyMatrixSequential(const Matrix4x4& mat)
{
    for (auto& p : points)
        p = mat.transformPoint(p);
}

void PointCloud::applyMatrixParallel(const Matrix4x4& mat)
{
    std::for_each(
        std::execution::par,
        points.begin(),
        points.end(),
        [&](Point3D& p)
        {
            p = mat.transformPoint(p);
        });
}

void PointCloud::downsample(int step)
{
    std::vector<Point3D> result;

    for (size_t i = 0; i < points.size(); i += step)
        result.push_back(points[i]);

    points = result;
}

size_t PointCloud::size() const
{
    return points.size();
}
