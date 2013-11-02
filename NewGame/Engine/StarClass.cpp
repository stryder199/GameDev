#include "StarClass.h"
#include "MeshClass.h"
#include "LightClass.h"

StarClass::StarClass(float posx, float posy, float posz){
	m_mesh = 0;
	m_lightSource = 0;
	m_pos = XMFLOAT3(posx, posy, posz);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(0.025f, 0.025f, 0.025f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotVel = XMFLOAT3(0.0f, 0.0025f, 0.0f);
}

StarClass::~StarClass(){
}

bool StarClass::Initialize(MeshClass* objMesh)
{
	bool result;

	m_mesh = objMesh;

	// Create the light object.
	m_lightSource = new LightClass();
	if (!m_lightSource)
		return false;
	// Initialize the light object.
	m_lightSource->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightSource->SetDirection(0.0f, 0.0f, -1.0f);

	//Initialize the vertex and index buffers that hold the geometry for the triangle.
	result = ModelClass::InitializeBuffers();
	if (!result)
		return false;

	return true;
}

void StarClass::Shutdown()
{
	return;
}

bool StarClass::Render(ShaderControllerClass* shader)
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

bool StarClass::PreProcessing()
{
	//bool result; not used

	m_rot.x += m_rotVel.x;
	m_rot.y += m_rotVel.y;
	m_rot.z += m_rotVel.z;
	ConstrainRotation();

	CalculateWorldMatrix();
	return true;
}