#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>

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

    void Initialize(MeshClass* mesh, LightClass *lightSource, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale);
    void Shutdown();

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

private:
    DirectX::XMFLOAT2 m_screenPos;
};
