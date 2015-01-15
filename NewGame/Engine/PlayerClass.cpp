#include "PlayerClass.h"
#include "ModelClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "BulletClass.h"
#include "ShaderControllerClass.h"
#include "Timer.h"
#include "MeshControllerClass.h"

PlayerClass* PlayerClass::m_pInstance = NULL;
mutex PlayerClass::instanceMutex;

PlayerClass::PlayerClass()
{
	m_mesh = 0;
	m_bulletMesh = 0;
	m_lightSource = 0;
	m_isWeaponFiring = false;
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_enginePower = 0.0f;
	m_totalHealth = 100;
	m_health = 100;
	m_totalShields = 100;
	m_shields = 100;
	m_torpedos = 50;
	m_totalEnergy = 100;
	m_energy = 100;
	m_energyCost = 2;
}

PlayerClass* PlayerClass::getInstance()
{
	instanceMutex.lock();
	if (!m_pInstance)
		m_pInstance = new PlayerClass();
	instanceMutex.unlock();

	return m_pInstance;
}

PlayerClass::~PlayerClass(){
}

bool PlayerClass::Initialize( MeshClass* objMesh, XMFLOAT3 pos, XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos)
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

void PlayerClass::Shutdown()
{
	if (m_lightSource)
	{
		delete m_lightSource;
		m_lightSource = 0;
	}
}

void PlayerClass::SpawnBullet(XMFLOAT3 spawnPos)
{
	BulletClass* bullet = new BulletClass();
	bullet->Initialize(m_bulletMesh, spawnPos, m_dir);
	m_allBullets.push_back(bullet);
}

void PlayerClass::FireWeapon()
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

void PlayerClass::StartWeaponFiring()
{
	if (!m_weaponReloadTimer.is_started() && m_energy >= m_energyCost)
	{
		FireWeapon();
	}
}

void PlayerClass::Render(ShaderControllerClass* shader)
{
	vector<BulletClass*>::iterator bullet;
	for (bullet = m_allBullets.begin(); bullet != m_allBullets.end(); ++bullet)
	{
		(*bullet)->Render(shader);
	}

	ModelClass::RenderBuffers(shader);
}

void PlayerClass::PreProcessing()
{
	vector<int> deadBullets = vector<int>();
	float maxThrust = 0.01f;

	m_rot.x += m_rotVel.x;
	m_rot.y += m_rotVel.y;
	m_rot.z += m_rotVel.z;
	ConstrainRotation();

	CalculateDirection();

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

	if (m_enginePower == 1.0f || m_enginePower == -1.0f)
	{
		m_vel.x = m_dir.x * maxThrust;
		m_vel.y = m_dir.y * maxThrust;
		m_vel.z = m_dir.z * maxThrust;

		if (m_enginePower == 1.0f)
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

	CalculateWorldMatrix();

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
			(*bullet)->PreProcessing();
		}
		
		count++;
	}
	vector<int>::reverse_iterator deadBulletIndex;
	for (deadBulletIndex = deadBullets.rbegin(); deadBulletIndex != deadBullets.rend(); ++deadBulletIndex)
	{
		BulletClass * deadBullet = m_allBullets[(*deadBulletIndex)];
		m_allBullets.erase(m_allBullets.begin() + (*deadBulletIndex));
		deadBullet->Shutdown();
		delete deadBullet;
		deadBullet = 0;
	}
}

void PlayerClass::SetVelX(float x)
{
	m_vel.x = x;
}

void PlayerClass::SetVelY(float y)
{
	m_vel.y = y;
}

void PlayerClass::SetVelZ(float z)
{
	m_vel.z = z;
}

void PlayerClass::SetRotVelX(float x)
{
	m_rotVel.x = x;
}
void PlayerClass::SetRotVelY(float y)
{
	m_rotVel.y = y;
}

void PlayerClass::SetRotVelZ(float z)
{
	m_rotVel.z = z;
}

void PlayerClass::SetEnginePower(float power)
{
	m_enginePower = power;
}

float PlayerClass::GetEnginePower()
{
	return m_enginePower;
}

int PlayerClass::GetTotalHealth()
{
	return m_totalHealth;
}

int PlayerClass::GetHealth()
{
	return m_health;
}

int PlayerClass::GetTotalShields()
{
	return m_totalShields;
}

int PlayerClass::GetShields()
{
	return m_shields;
}

int PlayerClass::GetTorpedos()
{
	return m_torpedos;
}

int PlayerClass::GetTotalEnergy()
{
	return m_totalEnergy;
}

int PlayerClass::GetEnergy()
{
	return m_energy;
}