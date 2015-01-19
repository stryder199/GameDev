#pragma once

//////////////
// INCLUDES //
#include <vector>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"
#include "Timer.h"

/////////////////
// FORWARD DEC //
class BulletClass;

using namespace DirectX;

class PlayerClass : 
    public ModelClass
{
public:
    ~PlayerClass();
    static PlayerClass* getInstance();

    void Initialize(MeshClass* mesh, XMFLOAT3 pos, XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos);
    void Shutdown();

    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

    void SpawnBullet(XMFLOAT3 spawnPos);
    void FireWeapon();
    void StartWeaponFiring();

    float GetEnginePower();
    int GetTotalHealth();
    int GetHealth();
    int GetTotalShields();
    int GetShields();
    int GetTorpedos();
    int GetTotalEnergy();
    int GetEnergy();

    void SetEnginePower(float x);
    void SetVelX(float x);
    void SetVelY(float y);
    void SetVelZ(float z);
    void SetRotVelX(float x);
    void SetRotVelY(float y);
    void SetRotVelZ(float z);
private:
    PlayerClass();
    PlayerClass(const PlayerClass&);
    PlayerClass& operator=(const PlayerClass&) {};

    static PlayerClass* m_pInstance;
    static mutex instanceMutex;

    bool m_isWeaponFiring;
    Timer m_weaponReloadTimer, m_weaponPulseTimer;
    int m_weaponPulseCount, m_weaponPulseMaxCount;
    XMFLOAT3 m_vel, m_rotVel;
    vector<BulletClass*> m_allBullets;
    MeshClass *m_bulletMesh;
    float m_enginePower;
    int m_totalHealth, m_health;
    int m_totalShields, m_shields;
    int m_torpedos;
    int m_totalEnergy, m_energy, m_energyCost;
};