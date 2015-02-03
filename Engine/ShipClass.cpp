#include "ShipClass.h"
#include "ModelClass.h"
#include "MeshClass.h"
#include "LightClass.h"
#include "BulletClass.h"
#include "Timer.h"
#include "MeshControllerClass.h"
#include "VectorHelpers.h"

ShipClass::ShipClass()
    : ModelClass()
{
    m_targetShip = 0;
    m_bulletMesh = 0;
    m_isWeaponFiring = false;
    m_vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rotVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_enginePower = CommonEnums::EnginePower::Stop;
    m_totalHealth = 100;
    m_health = 100;
    m_totalShields = 100;
    m_shields = 100;
    m_torpedos = 50;
    m_totalEnergy = 100;
    m_energy = 100;
    m_energyCost = 2;
}

ShipClass::~ShipClass()
{
}

void ShipClass::Initialize(MeshClass* objMesh, XMFLOAT3 pos, XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos)
{
    m_mesh = objMesh;
    m_pos = pos;
    m_scale = scale;
    m_totalHealth = totalHealth;
    m_health = m_totalHealth;
    m_totalShields = totalShields;
    m_shields = m_totalShields;
    m_totalEnergy = totalEnergy;
    m_energy = m_totalEnergy;
    m_torpedos = torpedos;
    m_energyCost = energyCost;

    m_bulletMesh = MeshControllerClass::getInstance()->getMesh("bulletMesh");

    // Create the light object.
    m_lightSource = new LightClass();
    // Initialize the light object.
    m_lightSource->SetAmbientColor(0.4f, 0.4f, 0.4f, 1.0f);
    m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightSource->SetDirection(0.0f, 0.0f, 1.0f);

    m_allBullets = vector<BulletClass*>();

    m_weaponReloadTimer = Timer();
    m_weaponPulseTimer = Timer();
    m_weaponPulseMaxCount = 3;
    m_weaponPulseCount = 0;
}

void ShipClass::Shutdown()
{
    if (m_lightSource)
    {
        delete m_lightSource;
        m_lightSource = 0;
    }
}

void ShipClass::SpawnBullet(XMFLOAT3 spawnPos)
{
    BulletClass* bullet = new BulletClass();
    bullet->Initialize(m_bulletMesh, spawnPos, m_dir);
    m_allBullets.push_back(bullet);
}

void ShipClass::FireWeapon()
{
    m_isWeaponFiring = true;
    m_energy -= m_energyCost;
    vector<XMFLOAT3>::iterator gun;
    for (gun = m_mesh->getGuns()->begin(); gun != m_mesh->getGuns()->end(); ++gun)
    {
        XMFLOAT4 gunPos = XMFLOAT4((*gun).x, (*gun).y, (*gun).z, 1.0f);
        XMVECTOR gunPosVec = XMLoadFloat4(&gunPos);
        gunPosVec = XMVector4Transform(gunPosVec, XMLoadFloat4x4(&m_worldMatrix));
        XMFLOAT3 pos;
        XMStoreFloat3(&pos, gunPosVec);
        SpawnBullet(pos);
    }
    if (m_weaponPulseCount == 0)
    {
        m_weaponReloadTimer.start();
    }

    m_weaponPulseTimer.start();
    m_weaponPulseCount++;
}

void ShipClass::StartWeaponFiring()
{
    if (!m_weaponReloadTimer.is_started() && m_energy >= m_energyCost)
    {
        FireWeapon();
    }
}

void ShipClass::FlyTowardsTarget()
{
    if (m_targetShip != nullptr)
    {
        SetEnginePower(CommonEnums::EnginePower::Forward);
        if (VectorHelpers::IsToLeft(m_pos, m_dir, m_targetShip->getPosition()))
        {
            GoLeft(false);
        }
        // else to right
        else
        {
            GoRight(false);
        }
    }
}

void ShipClass::GoRight(bool isPlayer)
{
    m_rotVel.x = 0.0f;
    if (isPlayer)
    {
        m_rotVel.y = (XM_PI / 100);
    }
    else
    {
        m_rotVel.y = (XM_PI / 1000);
    }
    m_rotVel.z = 0.0f;
}

void ShipClass::GoLeft(bool isPlayer)
{
    m_rotVel.x = 0.0f;
    if (isPlayer)
    {
        m_rotVel.y = -1.0f * (XM_PI / 100);
    }
    else
    {
        m_rotVel.y = -1.0f * (XM_PI / 1000);
    }
    m_rotVel.z = 0.0f;
}

void ShipClass::Stop()
{
    m_rotVel.x = 0.0f;
    m_rotVel.y = 0.0f;
    m_rotVel.z = 0.0f;
}

void ShipClass::Render(ShaderControllerClass* shader)
{
    vector<BulletClass*>::iterator bullet;
    for (bullet = m_allBullets.begin(); bullet != m_allBullets.end(); ++bullet)
    {
        (*bullet)->Render(shader);
    }

    RenderBuffers(shader);
}

void ShipClass::PreProcessing()
{
    vector<int> deadBullets = vector<int>();
    float maxThrust = 0.01f;

    m_rot.x += m_rotVel.x;
    m_rot.y += m_rotVel.y;
    m_rot.z += m_rotVel.z;

    if (m_isWeaponFiring)
    {
        if (m_weaponReloadTimer.get_ticks() > 1000)
        {
            m_isWeaponFiring = false;
            m_weaponReloadTimer.stop();
            m_weaponPulseTimer.stop();
            m_weaponPulseCount = 0;
        }
        if (m_weaponPulseTimer.get_ticks() > 100 && m_weaponPulseCount < m_weaponPulseMaxCount)
        {
            FireWeapon();
        }
    }

    if (m_enginePower == CommonEnums::EnginePower::Forward || m_enginePower == CommonEnums::EnginePower::Reverse)
    {
        m_vel.x = m_dir.x * maxThrust;
        m_vel.y = m_dir.y * maxThrust;
        m_vel.z = m_dir.z * maxThrust;

        if (m_enginePower == CommonEnums::EnginePower::Reverse)
        {
            m_vel.x *= -1.0f;
            m_vel.z *= -1.0f;
        }
        m_vel.y = 0.0f;
    }
    else
    {
        m_vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
    }

    XMVECTOR result = XMVectorAdd(XMLoadFloat3(&m_pos), XMLoadFloat3(&m_vel));
    XMStoreFloat3(&m_pos, result);

    vector<BulletClass*>::iterator bullet;
    int count = 0;
    for (bullet = m_allBullets.begin(); bullet != m_allBullets.end(); ++bullet)
    {
        if ((*bullet)->GetTimeAlive() > 10000)
        {
            deadBullets.push_back(count);
        }
        else
        {
            (*bullet)->ModelPreProcessing();
        }

        count++;
    }
    vector<int>::reverse_iterator deadBulletIndex;
    for (deadBulletIndex = deadBullets.rbegin(); deadBulletIndex != deadBullets.rend(); ++deadBulletIndex)
    {
        BulletClass *deadBullet = m_allBullets[(*deadBulletIndex)];
        m_allBullets.erase(m_allBullets.begin() + (*deadBulletIndex));
        deadBullet->Shutdown();
        delete deadBullet;
    }

    FlyTowardsTarget();
}

void ShipClass::SetEnginePower(CommonEnums::EnginePower power)
{
    m_enginePower = power;
}

CommonEnums::EnginePower ShipClass::GetEnginePower()
{
    return m_enginePower;
}

int ShipClass::GetTotalHealth()
{
    return m_totalHealth;
}

int ShipClass::GetHealth()
{
    return m_health;
}

int ShipClass::GetTotalShields()
{
    return m_totalShields;
}

int ShipClass::GetShields()
{
    return m_shields;
}

int ShipClass::GetTorpedos()
{
    return m_torpedos;
}

int ShipClass::GetTotalEnergy()
{
    return m_totalEnergy;
}

int ShipClass::GetEnergy()
{
    return m_energy;
}

ShipClass* ShipClass::GetTargetShip()
{
    return m_targetShip;
}

void ShipClass::SetNewTarget(ShipClass* target)
{
    m_targetShip = target;
}