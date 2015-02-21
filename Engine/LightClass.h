#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //

class LightClass
{
public:
    LightClass();
    ~LightClass();

    void SetAmbientColor(float, float, float, float);
    void SetDiffuseColor(float, float, float, float);
    void SetDirection(float, float, float);

    DirectX::XMFLOAT4 GetAmbientColor();
    DirectX::XMFLOAT4 GetDiffuseColor();
    DirectX::XMFLOAT3 GetDirection();

private:
    DirectX::XMFLOAT4 m_ambientColor;
    DirectX::XMFLOAT4 m_diffuseColor;
    DirectX::XMFLOAT3 m_direction;
};