//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class MeshClass;
class ShaderControllerClass;
class TextureClass;
class LightClass;
class BulletClass;
class Timer;

//=================================
// included dependencies
#include "ModelClass.h"
#include "Timer.h"
#include <vector>

using namespace DirectX;

class PlayerClass : public ModelClass{
public:
	~PlayerClass();
	static PlayerClass* getInstance();

	bool Initialize(MeshClass* mesh);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);

	void SpawnBullet(XMFLOAT3 spawnPos);
	void FireWeapon();
	void StartWeaponFiring();

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
	PlayerClass& operator=(PlayerClass& const) {};

	static PlayerClass* m_pInstance;

	virtual bool PreProcessing();

	bool m_isWeaponFiring;
	Timer m_weaponReloadTimer, m_weaponPulseTimer;
	int m_weaponPulseCount, m_weaponPulseMaxCount;
	float engine_power;
	XMFLOAT3 m_vel, m_rotVel;
	std::vector<BulletClass*> m_allBullets;
	MeshClass *m_bulletMesh;
};