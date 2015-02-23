#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
#include <memory>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //

class BitmapClass 
    : public ModelClass
{
public:
    BitmapClass();
    ~BitmapClass();

    void Initialize(std::shared_ptr<MeshClass> mesh, LightClass *lightSource, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale);

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

private:
    DirectX::XMFLOAT2 m_screenPos;
};
