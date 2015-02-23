#pragma once

//////////////
// INCLUDES //
#include <memory>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //

class PlanetClass :
    public ModelClass
{
public:
    PlanetClass();
    ~PlanetClass();

    void Initialize(std::shared_ptr<MeshClass> mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel);

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();
};

