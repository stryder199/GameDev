#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class MeshClass;
class ShaderControllerClass;
class LightClass;

class ModelClass{
public:
    ModelClass();

    virtual void Render(ShaderControllerClass* shader) = 0;
    void ModelPreProcessing();
    virtual void Shutdown() = 0;

    DirectX::XMFLOAT3 getPosition();
    DirectX::XMFLOAT3 getRotation();
    DirectX::XMFLOAT3 getScale();
    DirectX::XMFLOAT3 getPointPosition();
    DirectX::XMFLOAT3 getDirection();
    float getBasicCollisionCircleRadius();

    static bool sphereSphereCollision(DirectX::XMFLOAT3 p1, float r1, DirectX::XMFLOAT3 p2, float r2);

protected:
    virtual void PreProcessing() = 0;
    void RenderBuffers(ShaderControllerClass* shader);
    void CalculateWorldMatrix();
    void ConstrainRotation();

    MeshClass *m_mesh;
    LightClass *m_lightSource;
    DirectX::XMFLOAT3 m_pos, m_point_pos, m_rot, m_scale, m_dir, m_vel, m_rotVel;
    DirectX::XMFLOAT4 m_color;
    DirectX::XMFLOAT4X4 m_worldMatrix;
};