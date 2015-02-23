#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
#include <memory>

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

    DirectX::XMFLOAT3 getPosition();
    DirectX::XMFLOAT3 getRotation();
    DirectX::XMFLOAT3 getScale();
    DirectX::XMFLOAT3 getPointPosition();
    DirectX::XMFLOAT3 getDirection();
    DirectX::XMFLOAT3 getVelocityDirection();
    float getBasicCollisionCircleRadius();

protected:
    virtual void PreProcessing() = 0;
    void RenderBuffers(ShaderControllerClass* shader);

    std::shared_ptr<MeshClass> m_mesh;
    LightClass *m_lightSource;
    DirectX::XMFLOAT3 m_pos, m_point_pos, m_rot, m_scale, m_vel, m_rotVel;
    DirectX::XMFLOAT4 m_color;
    DirectX::XMFLOAT4X4 m_worldMatrix;

private:
    // Calculated vectors
    DirectX::XMFLOAT3 m_dir, m_dirVel;

    void CalculateWorldMatrix();
};