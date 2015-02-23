#include "CameraClass.h"
#include "ShipClass.h"
#include "VectorHelpers.h"

using namespace std;
using namespace DirectX;

CameraClass* CameraClass::m_pInstance = nullptr;
mutex CameraClass::instanceMutex;

CameraClass::CameraClass()
{
    m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_viewMatrix = XMFLOAT4X4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

CameraClass* CameraClass::getInstance()
{
    instanceMutex.lock();
    if (!m_pInstance)
        m_pInstance = new CameraClass();
    instanceMutex.unlock();

    return m_pInstance;
}

void CameraClass::Initialize()
{
    m_point_pos.z = -0.15;
}

void CameraClass::Shutdown()
{
    return;
}

void CameraClass::Render(std::shared_ptr<ShipClass> player)
{
    XMVECTOR up, position, lookAt;
    XMFLOAT3 playerOrigin;
    XMMATRIX positionRotMatrixX, positionRotMatrixY, positionRotMatrixZ;
    XMMATRIX worldMatrix;
    XMMATRIX pointTranslationMatrix, translationMatrix;

    if (player != nullptr)
    {
        playerOrigin = player->getPosition();
        m_rot = player->getRotation();
    }
    else
    {
        playerOrigin = XMFLOAT3(0.0f, 0.0f, 0.0f);
        m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    }

    VectorHelpers::ConstrainRotation(&m_rot);
    VectorHelpers::CalculateDirection(&m_rot, &m_dir);

    positionRotMatrixX = XMMatrixRotationX(m_rot.x);
    positionRotMatrixY = XMMatrixRotationY(m_rot.y);
    positionRotMatrixZ = XMMatrixRotationZ(m_rot.z);

    pointTranslationMatrix = XMMatrixTranslation(m_point_pos.x, m_point_pos.y, m_point_pos.z);
    translationMatrix = XMMatrixTranslation(playerOrigin.x, playerOrigin.y, playerOrigin.z);

    worldMatrix = XMMatrixIdentity();
    worldMatrix *= pointTranslationMatrix;
    worldMatrix *= positionRotMatrixX;
    worldMatrix *= positionRotMatrixY;
    worldMatrix *= positionRotMatrixZ;
    worldMatrix *= translationMatrix;

    position = XMVector4Transform(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), worldMatrix);

    //Setup the vector that points upwards
    up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    lookAt = XMVectorSet(playerOrigin.x, playerOrigin.y, playerOrigin.z, 0.0f) - position;
    lookAt = XMVector3Normalize(lookAt);

    XMFLOAT3 pos;
    XMStoreFloat3(&pos, position);
    pos.y += 0.02;
    position = XMLoadFloat3(&pos);

    //Translate the rotated camera position to the location of the viewer
    lookAt = position + lookAt;

    XMStoreFloat3(&m_pos, position);
    //Finally create the view matrix from the three updates vectors
    XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookAtLH(position, lookAt, up));
}

XMFLOAT4X4 CameraClass::GetViewMatrix()
{
    return m_viewMatrix;
}

XMFLOAT3 CameraClass::getPosition()
{
    return m_pos;
}

XMFLOAT3 CameraClass::getRotation()
{
    return m_rot;
}

XMFLOAT3 CameraClass::getScale()
{
    return m_scale;
}

XMFLOAT3 CameraClass::getPointPosition()
{
    return m_point_pos;
}

XMFLOAT3 CameraClass::getDirection()
{
    return m_dir;
}