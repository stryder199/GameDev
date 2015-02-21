#include "BulletClass.h"
#include "LightClass.h"
#include "CameraClass.h"
#include "Timer.h"

using namespace DirectX;

BulletClass::BulletClass()
    : ModelClass()
{
    m_life = nullptr;
}


BulletClass::~BulletClass()
{
}

void BulletClass::Initialize(MeshClass* objMesh, XMFLOAT3 pos, XMFLOAT3 dir, XMFLOAT3 scale)
{
    m_lightSource = new LightClass();
    m_lightSource->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightSource->SetDirection(0.0f, 0.0f, 1.0f);

    m_mesh = objMesh;
    m_pos = pos;
    m_dir = dir;
    m_scale = scale;

    m_life = new Timer();
    m_life->start();
}

void BulletClass::Shutdown()
{
    if (!m_life)
    {
        delete m_life;
        m_life = 0;
    }
}

void BulletClass::Render(ShaderControllerClass* shader)
{
    RenderBuffers(shader);
}

void BulletClass::PreProcessing()
{
    float bulletSpeed = 0.003f;

    m_vel.x = m_dir.x * bulletSpeed;
    m_vel.y = m_dir.y * bulletSpeed;
    m_vel.z = m_dir.z * bulletSpeed;
}

int BulletClass::GetTimeAlive()
{
    return m_life->get_ticks();
}