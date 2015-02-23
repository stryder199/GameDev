#include <DirectXMath.h>

class VectorHelpers
{
public:
    static bool IsToLeft(DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 sourceDirection, DirectX::XMFLOAT3 target);
    static bool IsToRight(DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 sourceDirection, DirectX::XMFLOAT3 target);
    static bool IsBehind(DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 sourceDirection, DirectX::XMFLOAT3 target);
    static bool IsInFront(DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 sourceDirection, DirectX::XMFLOAT3 target);
    static float Distance(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
    static DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 a);
    static void Normalize(DirectX::XMFLOAT3* a);
    static void CalculateDirection(const DirectX::XMFLOAT3* rot, DirectX::XMFLOAT3* dir);
    static void CalculateVelocityDirection(const DirectX::XMFLOAT3* oldPosition, const DirectX::XMFLOAT3* newPosition, DirectX::XMFLOAT3* dirVel);
    static void ConstrainRotation(DirectX::XMFLOAT3* rot);
    static bool SphereSphereCollision(DirectX::XMFLOAT3 p1, float r1, DirectX::XMFLOAT3 p2, float r2);
    static void ZeroVector(DirectX::XMFLOAT2* a);
    static void ZeroVector(DirectX::XMFLOAT3* a);
    static void ZeroVector(DirectX::XMFLOAT4* a);
    static bool Equal(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b);
    static bool Equal(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
    static bool Equal(DirectX::XMFLOAT4 a, DirectX::XMFLOAT4 b);
};