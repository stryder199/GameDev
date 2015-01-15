#include "PlanetClass.h"
#include "ModelClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"

PlanetClass::PlanetClass()
{
	m_mesh = 0;
	m_lightSource = 0;
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


PlanetClass::~PlanetClass()
{
}

void PlanetClass::Initialize(MeshClass* objMesh, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel)
{
	m_mesh = objMesh;
	m_pos = pos;
	m_scale = scale;
	m_rotVel = rotVel;

	// Create the light object.
	m_lightSource = new LightClass();
	// Initialize the light object.
	m_lightSource->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightSource->SetDirection(0.0f, 0.0f, 1.0f);
}

void PlanetClass::Shutdown()
{
	if (m_lightSource)
	{
		delete m_lightSource;
		m_lightSource = 0;
	}

	return;
}

void PlanetClass::Render(ShaderControllerClass* shader)
{
	ModelClass::RenderBuffers(shader);
}

void PlanetClass::PreProcessing()
{
	m_rot.x += m_rotVel.x;
	m_rot.y += m_rotVel.y;
	m_rot.z += m_rotVel.z;
	ConstrainRotation();

	CalculateWorldMatrix();
}