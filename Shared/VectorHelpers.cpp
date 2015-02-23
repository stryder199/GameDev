#include "VectorHelpers.h"
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

bool VectorHelpers::IsToLeft(XMFLOAT3 source, XMFLOAT3 sourceDirection, XMFLOAT3 target)
{
    XMVECTOR sourceV = XMLoadFloat3(&source);
    XMVECTOR targetV = XMLoadFloat3(&target);
    XMVECTOR sourceDirectionV = XMLoadFloat3(&sourceDirection);

    // Move the source to the origin
    // Which will reorient the target.
    targetV = XMVectorSubtract(targetV, sourceV);

    // Normalize
    targetV = XMPlaneNormalize(targetV);
    sourceDirectionV = XMPlaneNormalize(sourceDirectionV);

    XMMATRIX yRot = XMMatrixRotationY(-1 * XM_PIDIV2);
    sourceDirectionV = XMVector4Transform(sourceDirectionV, yRot);

    // Compare the Direction of the source to the direction of the target relative to the source
    float result = 0.0f;
    XMStoreFloat(&result, XMPlaneDotCoord(targetV, sourceDirectionV));

    if (result < 0)
    {
        return false;
    }

    return true;
}

bool VectorHelpers::IsToRight(XMFLOAT3 source, XMFLOAT3 sourceDirection, XMFLOAT3 target)
{
    XMVECTOR sourceV = XMLoadFloat3(&source);
    XMVECTOR targetV = XMLoadFloat3(&target);
    XMVECTOR sourceDirectionV = XMLoadFloat3(&sourceDirection);

    // Move the source to the origin
    // Which will reorient the target.
    targetV = XMVectorSubtract(targetV, sourceV);

    // Normalize
    targetV = XMPlaneNormalize(targetV);
    sourceDirectionV = XMPlaneNormalize(sourceDirectionV);

    XMMATRIX yRot = XMMatrixRotationY(-1 * XM_PIDIV2);
    sourceDirectionV = XMVector4Transform(sourceDirectionV, yRot);

    // Compare the Direction of the source to the direction of the target relative to the source
    float result = 0.0f;
    XMStoreFloat(&result, XMPlaneDotCoord(targetV, sourceDirectionV));

    if (result >= 0)
    {
        return false;
    }

    return true;
}

bool VectorHelpers::IsBehind(XMFLOAT3 source, XMFLOAT3 sourceDirection, XMFLOAT3 target)
{
    XMVECTOR sourceV = XMLoadFloat3(&source);
    XMVECTOR targetV = XMLoadFloat3(&target);
    XMVECTOR sourceDirectionV = XMLoadFloat3(&sourceDirection);

    // Move the source to the origin
    // Which will reorient the target.
    targetV = XMVectorSubtract(targetV, sourceV);

    // Normalize
    targetV = XMPlaneNormalize(targetV);
    sourceDirectionV = XMPlaneNormalize(sourceDirectionV);

    // Compare the Direction of the source to the direction of the target relative to the source
    float result = 0.0f;
    XMStoreFloat(&result, XMPlaneDotCoord(targetV, sourceDirectionV));

    if (result < 0)
    {
        return true;
    }

    return false;
}

bool VectorHelpers::IsInFront(XMFLOAT3 source, XMFLOAT3 sourceDirection, XMFLOAT3 target)
{
    XMVECTOR sourceV = XMLoadFloat3(&source);
    XMVECTOR targetV = XMLoadFloat3(&target);
    XMVECTOR sourceDirectionV = XMLoadFloat3(&sourceDirection);

    // Move the source to the origin
    // Which will reorient the target.
    targetV = XMVectorSubtract(targetV, sourceV);

    // Normalize
    targetV = XMPlaneNormalize(targetV);
    sourceDirectionV = XMPlaneNormalize(sourceDirectionV);

    // Compare the Direction of the source to the direction of the target relative to the source
    float result = 0.0f;
    XMStoreFloat(&result, XMPlaneDotCoord(targetV, sourceDirectionV));

    if (result >= 0)
    {
        return true;
    }

    return false;
}

float VectorHelpers::Distance(XMFLOAT3 a, XMFLOAT3 b)
{
    XMVECTOR diff = XMVectorSubtract(XMLoadFloat3(&b), XMLoadFloat3(&a));
    float dot = 0.0f;
    XMStoreFloat(&dot, XMPlaneDotCoord(diff, diff));
    return sqrtf(dot);
}

XMFLOAT3 Normalize(XMFLOAT3 a)
{
    XMFLOAT3 result;
    XMVECTOR normalizedVector = XMVector3Normalize(XMLoadFloat3(&a));
    XMStoreFloat3(&result, normalizedVector);
    return result;
}

void VectorHelpers::Normalize(XMFLOAT3* a)
{
    XMVECTOR normalizedVector = XMVector3Normalize(XMLoadFloat3(a));
    XMStoreFloat3(a, normalizedVector);
}

void VectorHelpers::CalculateDirection(const XMFLOAT3* rot, XMFLOAT3* dir)
{
    if (rot->y >= 0.0f && rot->y < XM_PIDIV2)
    {
        dir->x = (rot->y / XM_PIDIV2);
    }
    else if (rot->y >= XM_PIDIV2 && rot->y < ((3.0f*XM_PI) / 2.0f))
    {
        dir->x = ((XM_PI - rot->y) / XM_PIDIV2);
    }
    else if (rot->y >= ((3.0f*XM_PI) / 2.0f) && rot->y < XM_2PI)
    {
        dir->x = ((rot->y - 2 * XM_PI) / XM_PIDIV2);
    }

    if (rot->y >= 0.0f && rot->y < XM_PI)
    {
        dir->z = ((XM_PIDIV2 - rot->y) / XM_PIDIV2);
    }
    else if (rot->y >= XM_PI && rot->y < XM_2PI)
    {
        dir->z = ((rot->y - ((3.0f*XM_PI) / 2.0f)) / XM_PIDIV2);
    }
    VectorHelpers::Normalize(dir);
}

void VectorHelpers::ConstrainRotation(XMFLOAT3* rot)
{
    if (rot->x >= 2 * XM_PI)
        rot->x = rot->x - 2 * XM_PI;
    else if (rot->x < 0.0f)
        rot->x = 2 * XM_PI + rot->x;

    if (rot->y >= 2 * XM_PI)
        rot->y = rot->y - 2 * XM_PI;
    else if (rot->y < 0.0f)
        rot->y = 2 * XM_PI + rot->y;

    if (rot->z >= 2 * XM_PI)
        rot->z = rot->z - 2 * XM_PI;
    else if (rot->z < 0.0f)
        rot->z = 2 * XM_PI + rot->z;
}

bool VectorHelpers::SphereSphereCollision(XMFLOAT3 p1, float r1, XMFLOAT3 p2, float r2)
{
    XMVECTOR distance = XMVectorSubtract(XMLoadFloat3(&p1), XMLoadFloat3(&p2));
    XMVECTOR distanceLengthVec = XMVector3Length(distance);
    float distanceLength = 0.0f;
    XMStoreFloat(&distanceLength, distanceLengthVec);

    float sumradius = r1 + r2;

    // If the distance between 2 spheres is less then the sum of the radius's then we have a collision
    if (distanceLength < sumradius)
    {
        return true;
    }

    return false;
}

void VectorHelpers::CalculateVelocityDirection(const XMFLOAT3* oldPosition, const XMFLOAT3* newPosition, XMFLOAT3* dirVel)
{
    XMVECTOR distance = XMVectorSubtract(XMLoadFloat3(newPosition), XMLoadFloat3(oldPosition));
    distance = XMVector3Normalize(distance);
    XMStoreFloat3(dirVel, distance);
}

void VectorHelpers::ZeroVector(XMFLOAT2* a)
{
    a->x = 0.0f;
    a->y = 0.0f;
}

void VectorHelpers::ZeroVector(XMFLOAT3* a)
{
    a->x = 0.0f;
    a->y = 0.0f;
    a->z = 0.0f;
}

void VectorHelpers::ZeroVector(XMFLOAT4* a)
{
    a->x = 0.0f;
    a->y = 0.0f;
    a->z = 0.0f;
    a->w = 0.0f;
}

bool VectorHelpers::Equal(XMFLOAT2 a, XMFLOAT2 b)
{
    return a.x == b.x && a.y == b.y;
}

bool VectorHelpers::Equal(XMFLOAT3 a, XMFLOAT3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool VectorHelpers::Equal(XMFLOAT4 a, XMFLOAT4 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}