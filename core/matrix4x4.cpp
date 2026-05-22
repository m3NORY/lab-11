
#include "matrix4x4.h"

#include <cmath>

Matrix4x4::Matrix4x4()
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = 0;
}

Matrix4x4 Matrix4x4::identity()
{
    Matrix4x4 mat;

    for (int i = 0; i < 4; ++i)
        mat.m[i][i] = 1;

    return mat;
}

Matrix4x4 Matrix4x4::translation(double dx, double dy, double dz)
{
    Matrix4x4 mat = identity();

    mat.m[0][3] = dx;
    mat.m[1][3] = dy;
    mat.m[2][3] = dz;

    return mat;
}

Matrix4x4 Matrix4x4::rotationX(double angle)
{
    Matrix4x4 mat = identity();

    double r = angle * 3.14 / 180.0;

    mat.m[1][1] = cos(r);
    mat.m[1][2] = -sin(r);

    mat.m[2][1] = sin(r);
    mat.m[2][2] = cos(r);

    return mat;
}

Matrix4x4 Matrix4x4::rotationY(double angle)
{
    Matrix4x4 mat = identity();

    double r = angle * 3.14 / 180.0;

    mat.m[0][0] = cos(r);
    mat.m[0][2] = sin(r);

    mat.m[2][0] = -sin(r);
    mat.m[2][2] = cos(r);

    return mat;
}

Matrix4x4 Matrix4x4::rotationZ(double angle)
{
    Matrix4x4 mat = identity();

    double r = angle * 3.14 / 180.0;

    mat.m[0][0] = cos(r);
    mat.m[0][1] = -sin(r);

    mat.m[1][0] = sin(r);
    mat.m[1][1] = cos(r);

    return mat;
}

Matrix4x4 Matrix4x4::scale(double s)
{
    Matrix4x4 mat = identity();

    mat.m[0][0] = s;
    mat.m[1][1] = s;
    mat.m[2][2] = s;

    return mat;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
    Matrix4x4 r;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
                r.m[i][j] += m[i][k] * other.m[k][j];
        }
    }

    return r;
}

Point3D Matrix4x4::transformPoint(const Point3D& p) const
{
    Point3D r;

    r.x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
    r.y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
    r.z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];

    return r;
}
