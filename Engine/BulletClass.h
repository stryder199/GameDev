#pragma once

//////////////
// INCLUDES //
#include <math.h>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //
class Timer;

using namespace DirectX;

class BulletClass :
    public ModelClass
{
public:
    BulletClass();
    ~BulletClass();

    void Initialize(MeshClass* mesh, XMFLOAT3 pos, XMFLOAT3 dir);
    void Shutdown();

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

    int GetTimeAlive();
private:
    Timer* m_life;
    XMFLOAT3 m_vel;
};

