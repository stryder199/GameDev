#include "BulletClass.h"
#include "LightClass.h"
#include "CameraClass.h"
#include "Timer.h"

using namespace std;
using namespace DirectX;

BulletClass::BulletClass()
    : ModelClass()
{
}


BulletClass::~BulletClass()
{
}

void BulletClass::Initialize(shared_ptr<MeshClass> objMesh, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, int damage)
{
    m_lightSource = new LightClass();
    m_lightSource->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightSource->SetDirection(0.0f, 0.0f, 1.0f);

    m_mesh = objMesh;
    m_pos = pos;
    m_rot = rot;
    m_scale = scale;

    m_life = Timer();
    m_life.start();
    
    m_damage = damage;
}

void BulletClass::Render(ShaderControllerClass* shader)
{
    RenderBuffers(shader);
}

void BulletClass::PreProcessing()
{
    float bulletSpeed = 0.03f;

    m_vel.x = getDirection().x * bulletSpeed;
    m_vel.y = getDirection().y * bulletSpeed;
    m_vel.z = getDirection().z * bulletSpeed;
}

int BulletClass::GetTimeAlive()
{
    return m_life.get_ticks();
}

int BulletClass::GetDamage()
{
    return m_damage;
}