#pragma once

//////////////
// INCLUDES //

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //

class StarClass : 
    public ModelClass
{
public:
    StarClass();
    ~StarClass();
    
    void Initialize(MeshClass* objMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel);
    void Shutdown() override;

    void Render(ShaderControllerClass* shader) override;
    virtual void PreProcessing() override;
};