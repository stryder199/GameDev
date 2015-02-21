#include <DirectXMath.h>

class VectorHelpers
{
public:
    static bool IsToLeft(DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 sourceDirection, DirectX::XMFLOAT3 target)
    {
        DirectX::XMVECTOR sourceV = XMLoadFloat3(&source);
        DirectX::XMVECTOR targetV = XMLoadFloat3(&target);
        DirectX::XMVECTOR sourceDirectionV = XMLoadFloat3(&sourceDirection);

        // Move the source to the origin
        // Which will reorient the target.
        targetV = DirectX::XMVectorSubtract(targetV, sourceV);

        // Normalize
        targetV = DirectX::XMPlaneNormalize(targetV);
        sourceDirectionV = DirectX::XMPlaneNormalize(sourceDirectionV);

        DirectX::XMMATRIX yRot = DirectX::XMMatrixRotationY(-1 * DirectX::XM_PIDIV2);
        sourceDirectionV = XMVector4Transform(sourceDirectionV, yRot);

        // Compare the Direction of the source to the direction of the target relative to the source
        float result = 0.0f;
        DirectX::XMStoreFloat(&result, DirectX::XMPlaneDotCoord(targetV, sourceDirectionV));

        if (result < 0)
        {
            return false;
        }

        return true;
    }

    static bool IsToRight(DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 sourceDirection, DirectX::XMFLOAT3 target)
    {
        DirectX::XMVECTOR sourceV = XMLoadFloat3(&source);
        DirectX::XMVECTOR targetV = XMLoadFloat3(&target);
        DirectX::XMVECTOR sourceDirectionV = XMLoadFloat3(&sourceDirection);

        // Move the source to the origin
        // Which will reorient the target.
        targetV = DirectX::XMVectorSubtract(targetV, sourceV);

        // Normalize
        targetV = DirectX::XMPlaneNormalize(targetV);
        sourceDirectionV = DirectX::XMPlaneNormalize(sourceDirectionV);

        DirectX::XMMATRIX yRot = DirectX::XMMatrixRotationY(-1 * DirectX::XM_PIDIV2);
        sourceDirectionV = XMVector4Transform(sourceDirectionV, yRot);

        // Compare the Direction of the source to the direction of the target relative to the source
        float result = 0.0f;
        DirectX::XMStoreFloat(&result, DirectX::XMPlaneDotCoord(targetV, sourceDirectionV));

        if (result >= 0)
        {
            return false;
        }

        return true;
    }

    static bool IsBehind(DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 sourceDirection, DirectX::XMFLOAT3 target)
    {
        DirectX::XMVECTOR sourceV = XMLoadFloat3(&source);
        DirectX::XMVECTOR targetV = XMLoadFloat3(&target);
        DirectX::XMVECTOR sourceDirectionV = XMLoadFloat3(&sourceDirection);

        // Move the source to the origin
        // Which will reorient the target.
        targetV = DirectX::XMVectorSubtract(targetV, sourceV);

        // Normalize
        targetV = DirectX::XMPlaneNormalize(targetV);
        sourceDirectionV = DirectX::XMPlaneNormalize(sourceDirectionV);

        // Compare the Direction of the source to the direction of the target relative to the source
        float result = 0.0f;
        DirectX::XMStoreFloat(&result, DirectX::XMPlaneDotCoord(targetV, sourceDirectionV));

        if (result < 0)
        {
            return true;
        }

        return false;
    }

    static bool IsInFront(DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 sourceDirection, DirectX::XMFLOAT3 target)
    {
        DirectX::XMVECTOR sourceV = XMLoadFloat3(&source);
        DirectX::XMVECTOR targetV = XMLoadFloat3(&target);
        DirectX::XMVECTOR sourceDirectionV = XMLoadFloat3(&sourceDirection);

        // Move the source to the origin
        // Which will reorient the target.
        targetV = DirectX::XMVectorSubtract(targetV, sourceV);

        // Normalize
        targetV = DirectX::XMPlaneNormalize(targetV);
        sourceDirectionV = DirectX::XMPlaneNormalize(sourceDirectionV);

        // Compare the Direction of the source to the direction of the target relative to the source
        float result = 0.0f;
        DirectX::XMStoreFloat(&result, DirectX::XMPlaneDotCoord(targetV, sourceDirectionV));

        if (result >= 0)
        {
            return true;
        }

        return false;
    }

    static float Distance(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
    {
        DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(XMLoadFloat3(&b), XMLoadFloat3(&a));        
        float dot = 0.0f;
        DirectX::XMStoreFloat(&dot, DirectX::XMPlaneDotCoord(diff, diff));
        return sqrtf(dot);
    }
};