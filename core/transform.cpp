
#include "transform.h"

Matrix4x4 TransformOperation::toMatrix() const
{
    switch (type)
    {
    case TransformType::Translate:
        return Matrix4x4::translation(a, b, c);

    case TransformType::RotateX:
        return Matrix4x4::rotationX(a);

    case TransformType::RotateY:
        return Matrix4x4::rotationY(a);

    case TransformType::RotateZ:
        return Matrix4x4::rotationZ(a);

    case TransformType::Scale:
        return Matrix4x4::scale(a);
    }

    return Matrix4x4::identity();
}

QString TransformOperation::toString() const
{
    switch (type)
    {
    case TransformType::Translate:
        return QString("Translate(%1, %2, %3)").arg(a).arg(b).arg(c);

    case TransformType::RotateX:
        return QString("RotateX(%1)").arg(a);

    case TransformType::RotateY:
        return QString("RotateY(%1)").arg(a);

    case TransformType::RotateZ:
        return QString("RotateZ(%1)").arg(a);

    case TransformType::Scale:
        return QString("Scale(%1)").arg(a);
    }

    return "";
}
