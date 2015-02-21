#pragma once

//////////////
// INCLUDES //
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"
#include "CommonEnums.h"
#include "Timer.h"

/////////////////
// FORWARD DEC //
class BulletClass;

class ShipClass :
    public ModelClass
{
public:
    ShipClass();
    ~ShipClass();

    void Initialize(MeshClass* mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos);
    void Shutdown();

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

    void SpawnBullet(DirectX::XMFLOAT3 spawnPos);
    void FireWeapon();
    void StartWeaponFiring();
    void FlyTowardsTarget();

    CommonEnums::EnginePower GetEnginePower();
    int GetTotalHealth();
    int GetHealth();
    int GetTotalShields();
    int GetShields();
    int GetTorpedos();
    int GetTotalEnergy();
    int GetEnergy();
    ShipClass *GetTargetShip();
    
    void SetNewTarget(ShipClass* target);

    void GoRight(bool);
    void GoLeft(bool);
    void Stop();

    void SetEnginePower(CommonEnums::EnginePower x);
private:
    bool m_isWeaponFiring;
    Timer m_weaponReloadTimer, m_weaponPulseTimer;
    int m_weaponPulseCount, m_weaponPulseMaxCount;
    std::vector<BulletClass*> m_allBullets;
    MeshClass *m_bulletMesh;
    ShipClass *m_targetShip;
    CommonEnums::EnginePower m_enginePower;
    int m_totalHealth, m_health;
    int m_totalShields, m_shields;
    int m_torpedos;
    int m_totalEnergy, m_energy, m_energyCost;
};