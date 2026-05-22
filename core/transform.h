
#pragma once

#include "matrix4x4.h"

#include <QString>

enum class TransformType
{
    Translate,
    RotateX,
    RotateY,
    RotateZ,
    Scale
};

struct TransformOperation
{
    TransformType type;

    double a = 0;
    double b = 0;
    double c = 0;

    Matrix4x4 toMatrix() const;

    QString toString() const;
};
