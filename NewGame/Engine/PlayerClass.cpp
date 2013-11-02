#include "PlayerClass.h"
#include "ModelClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "BulletClass.h"
#include "ShaderControllerClass.h"
#include "Timer.h"

PlayerClass::PlayerClass(){
	m_mesh = 0;
	m_lightSource = 0;
	weaponTimer = 0;
	m_isWeaponFiring = false;
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(0.008f, 0.008f, 0.008f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

PlayerClass::~PlayerClass(){
}

bool PlayerClass::Initialize( MeshClass* objMesh )
{
	bool result;

	m_mesh = objMesh;

	// Create the light object.
	m_lightSource = new LightClass();
	if (!m_lightSource)
		return false;
	// Initialize the light object.
	m_lightSource->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightSource->SetDirection(0.0f, 0.0f, 1.0f);

	//Initialize the vertex and index buffers that hold the geometry for the triangle.
	result = ModelClass::InitializeBuffers();
	if(!result)
		return false;

	m_allBullets = std::vector<BulletClass*>();

	m_bulletMesh = new MeshClass();
	result = m_bulletMesh->Initialize("data/bullet.3dmodel");
	if (!result)
		return false;

	weaponTimer = new Timer();

	return true;
}

void PlayerClass::Shutdown()
{
	//Release the vertex and index buffers
	ModelClass::ShutdownBuffers();

	return;
}

void PlayerClass::SpawnBullet(XMFLOAT3 spawnPos)
{
	BulletClass* bullet = new BulletClass();
	bullet->Initialize(m_bulletMesh, spawnPos, m_dir);
	m_allBullets.push_back(bullet);
}

void PlayerClass::FireWeapon()
{
	if (!weaponTimer->is_started())
	{
		m_isWeaponFiring = true;
		vector<XMFLOAT3>::iterator gun;
		for (gun = m_mesh->getGuns()->begin(); gun != m_mesh->getGuns()->end(); ++gun)
		{
			XMVECTOR gunPos = XMLoadFloat3(&(*gun));
			gunPos = XMVector4Transform(gunPos, XMLoadFloat4x4(&m_worldMatrix));
			XMFLOAT3 pos;
			XMStoreFloat3(&pos, gunPos);
			SpawnBullet(pos);
		}
		
		weaponTimer->start();
	}
}

bool PlayerClass::Render(ShaderControllerClass* shader)
{
	bool result;
	vector<int> deadBullets = vector<int>();

	result = PreProcessing();
	if (!result)
		return false;

	//Generate the view matrix based on the camera's position
	CameraClass::getInstance()->Render(this);

	vector<BulletClass*>::iterator bullet;
	int count = 0;
	for (bullet = m_allBullets.begin(); bullet != m_allBullets.end(); ++bullet)
	{
		if ((*bullet)->GetTimeAlive() > 6000)
		{
			deadBullets.push_back(count);
		}
		(*bullet)->Render(shader);
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

	result = ModelClass::RenderBuffers(shader);
	if(!result)
		return false;

	return true;
}

bool PlayerClass::PreProcessing()
{
	//bool result; not used
	float maxThrust = 0.01f;

	m_rot.x += m_rotVel.x;
	m_rot.y += m_rotVel.y;
	m_rot.z += m_rotVel.z;
	ConstrainRotation();

	CalculateDirection();

	if (m_isWeaponFiring)
	{
		if (weaponTimer->get_ticks() > 500)
		{
			m_isWeaponFiring = false;
			weaponTimer->stop();
		}
	}

	if (engine_power == 1.0f || engine_power == -1.0f)
	{
		m_vel.x = m_dir.x * maxThrust;
		m_vel.y = m_dir.y * maxThrust;
		m_vel.z = m_dir.z * maxThrust;

		if (engine_power == 1.0f)
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
	return true;
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
	engine_power = power;
}