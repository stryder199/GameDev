#include "BulletClass.h"
#include "LightClass.h"
#include "CameraClass.h"
#include "Timer.h"

BulletClass::BulletClass()
{
	m_mesh = 0;
	m_life = 0;
	m_lightSource = 0;
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(0.03f, 0.03f, 0.03f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


BulletClass::~BulletClass()
{
}

bool BulletClass::Initialize(MeshClass* objMesh, XMFLOAT3 pos, XMFLOAT3 dir)
{
	bool result;

	m_lightSource = new LightClass();
	m_lightSource->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightSource->SetDirection(0.0f, 0.0f, 1.0f);

	m_mesh = objMesh;
	m_pos = pos;
	m_dir = dir;

	m_rot.y = atan2f(dir.z, dir.x);
	m_rot.y -= XM_PI / 2.0f;
	m_rot.y *= -1.0f;
	ConstrainRotation();

	m_life = new Timer();
	m_life->start();

	//Initialize the vertex and index buffers that hold the geometry for the triangle.
	result = ModelClass::InitializeBuffers();
	if (!result)
		return false;

	return true;
}

void BulletClass::Shutdown()
{
	if (!m_life)
	{
		delete m_life;
		m_life = 0;
	}

	return;
}

bool BulletClass::Render(ShaderControllerClass* shader)
{
	bool result;

	result = PreProcessing();
	if (!result)
		return false;

	result = ModelClass::RenderBuffers(shader);
	if (!result)
		return false;

	return true;
}

bool BulletClass::PreProcessing()
{
	//bool result; not used
	float bulletSpeed = 0.03f;

	XMVECTOR pos = XMLoadFloat3(&m_pos);
	XMVECTOR dir = XMLoadFloat3(&m_dir);
	XMVECTOR vel = XMVectorScale(dir, bulletSpeed);
	vel = XMVectorScale(vel, -1.0f);
	XMVECTOR result = XMVectorAdd(pos, vel);
	XMStoreFloat3(&m_pos, result);

	CalculateWorldMatrix();
	return true;
}

int BulletClass::GetTimeAlive()
{
	return m_life->get_ticks();
}