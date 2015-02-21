#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class ShipClass;

class CameraClass{
public:
    ~CameraClass() {};
    static CameraClass* getInstance();

    void Initialize();
    void Shutdown();
    
    void Render(ShipClass* player);

    DirectX::XMFLOAT4X4 GetViewMatrix();
    DirectX::XMFLOAT3 getPosition();
    DirectX::XMFLOAT3 getRotation();
    DirectX::XMFLOAT3 getScale();
    DirectX::XMFLOAT3 getPointPosition();
    DirectX::XMFLOAT3 getDirection();

    void ConstrainRotation();
    void CalculateDirection();
private:
    CameraClass();
    CameraClass(const CameraClass&) {};
    CameraClass& operator=(const CameraClass&) {};

    static CameraClass* m_pInstance;
    static std::mutex instanceMutex;

    DirectX::XMFLOAT3 m_pos, m_point_pos, m_rot, m_scale, m_dir;
    DirectX::XMFLOAT4X4 m_viewMatrix;

};