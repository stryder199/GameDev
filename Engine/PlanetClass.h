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

    void Initialize(MeshClass* mesh, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel);
    void Shutdown();

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();
private:
    XMFLOAT3 m_rotVel;
};

