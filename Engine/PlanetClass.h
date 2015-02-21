#pragma once

//////////////
// INCLUDES //

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

    void Initialize(MeshClass* mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel);
    void Shutdown();

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();
};

