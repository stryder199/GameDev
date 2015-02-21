#pragma once

//////////////
// INCLUDES //

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //
class Timer;

class BulletClass :
    public ModelClass
{
public:
    BulletClass();
    ~BulletClass();

    void Initialize(MeshClass* mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 scale);
    void Shutdown();

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

    int GetTimeAlive();
private:
    Timer* m_life;
};

