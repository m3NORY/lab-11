
#pragma once

#include "pointcloud.h"

class Matrix4x4
{
public:
    double m[4][4];

    Matrix4x4();

    static Matrix4x4 identity();

    static Matrix4x4 translation(double dx, double dy, double dz);

    static Matrix4x4 rotationX(double angle);

    static Matrix4x4 rotationY(double angle);

    static Matrix4x4 rotationZ(double angle);

    static Matrix4x4 scale(double s);

    Matrix4x4 operator*(const Matrix4x4& other) const;

    Point3D transformPoint(const Point3D& p) const;
};
