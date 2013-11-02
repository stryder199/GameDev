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
	m_pos = XMFLOAT3(0.0f, 0.0f, 10.0f);
	m_rot = XMFLOAT3(0.366519143f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(0.025f, 0.025f, 0.025f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotVel = XMFLOAT3(0.0f, 0.0025f, 0.0f);
}


PlanetClass::~PlanetClass()
{
}

bool PlanetClass::Initialize(MeshClass* objMesh)
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
	if (!result)
		return false;

	return true;
}

void PlanetClass::Shutdown()
{
	//Release the vertex and index buffers
	ModelClass::ShutdownBuffers();

	return;
}

bool PlanetClass::Render(ShaderControllerClass* shader)
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

bool PlanetClass::PreProcessing()
{
	//bool result; not used

	m_rot.x += m_rotVel.x;
	m_rot.y += m_rotVel.y;
	m_rot.z += m_rotVel.z;
	ConstrainRotation();

	CalculateWorldMatrix();
	return true;
}