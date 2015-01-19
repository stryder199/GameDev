#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //

using namespace DirectX;

class BitmapClass : public ModelClass
{
public:
    BitmapClass();
    ~BitmapClass();

    void Initialize(MeshClass* mesh, LightClass *lightSource, XMFLOAT2 pos, XMFLOAT2 scale);
    void Shutdown();

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

private:
    XMFLOAT2 m_screenPos;
};
