#pragma once

//////////////
// INCLUDES //
#include <memory>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"
#include "Timer.h"

/////////////////
// FORWARD DEC //

class BulletClass :
    public ModelClass
{
public:
    BulletClass();
    ~BulletClass();

    void Initialize(std::shared_ptr<MeshClass> mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale, int damage);

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

    int GetTimeAlive();
    int GetDamage();
private:
    Timer m_life;
    int m_damage;
};

