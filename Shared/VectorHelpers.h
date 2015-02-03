#include <DirectXMath.h>

using namespace DirectX;

class VectorHelpers
{
public:
    static bool IsToLeft(XMFLOAT3 source, XMFLOAT3 sourceDirection, XMFLOAT3 target)
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

    static bool IsToRight(XMFLOAT3 source, XMFLOAT3 sourceDirection, XMFLOAT3 target)
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

    static bool IsBehind(XMFLOAT3 source, XMFLOAT3 sourceDirection, XMFLOAT3 target)
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

    static bool IsInFront(XMFLOAT3 source, XMFLOAT3 sourceDirection, XMFLOAT3 target)
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

    static float Distance(XMFLOAT3 a, XMFLOAT3 b)
    {
        XMVECTOR diff = XMLoadFloat3(&b) - XMLoadFloat3(&a);
        float dot = 0.0f;
        XMStoreFloat(&dot, XMPlaneDotCoord(diff, diff));
        return sqrtf(dot);
    }
};