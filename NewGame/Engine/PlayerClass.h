//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class MeshClass;
class ShaderControllerClass;
class D3DClass;
class CameraClass;
class TextureClass;
class LightClass;
class BulletClass;
class Timer;

//=================================
// included dependencies
#include "ModelClass.h"
#include <vector>

using namespace DirectX;

class PlayerClass : public ModelClass{
public:
	PlayerClass();
	~PlayerClass();

	bool Initialize(MeshClass* mesh);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);

	void SpawnBullet(XMFLOAT3 spawnPos);
	void FireWeapon();

	void SetEnginePower(float x);
	void SetVelX(float x);
	void SetVelY(float y);
	void SetVelZ(float z);
	void SetRotVelX(float x);
	void SetRotVelY(float y);
	void SetRotVelZ(float z);
private:
	virtual bool PreProcessing();

	bool m_isWeaponFiring;
	Timer* weaponTimer;
	float engine_power;
	XMFLOAT3 m_vel, m_rotVel;
	std::vector<BulletClass*> m_allBullets;
	MeshClass *m_bulletMesh;
};