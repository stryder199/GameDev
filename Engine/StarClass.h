#pragma once

//////////////
// INCLUDES //
#include <memory>

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
    
    void Initialize(std::shared_ptr<MeshClass> objMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel);

    void Render(ShaderControllerClass* shader) override;
    virtual void PreProcessing() override;
};