#pragma once

//////////////
// INCLUDES //
#include <D3D11.h>
#include <DirectXMath.h>
#include <vector>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class MeshClass;
class ShaderControllerClass;
class LightClass;

using namespace DirectX;

class ModelClass{
public:
    virtual void Render(ShaderControllerClass* shader) = 0;
    virtual void PreProcessing() = 0;
    virtual void Shutdown() = 0;

    XMFLOAT3 getPosition();
    XMFLOAT3 getRotation();
    XMFLOAT3 getScale();
    XMFLOAT3 getPointPosition();
    XMFLOAT3 getDirection();
    float getBasicCollisionCircleRadius();

    static bool sphereSphereCollision(XMFLOAT3 p1, float r1, XMFLOAT3 p2, float r2);

protected:
    void RenderBuffers(ShaderControllerClass* shader);
    void CalculateWorldMatrix();
    void ConstrainRotation();
    void CalculateDirection();

    MeshClass* m_mesh;
    LightClass *m_lightSource;
    XMFLOAT3 m_pos, m_point_pos, m_rot, m_scale, m_dir;
    XMFLOAT4 m_color;
    XMFLOAT4X4 m_worldMatrix;
};