#include "StarClass.h"
#include "MeshClass.h"
#include "LightClass.h"

StarClass::StarClass(float posx, float posy, float posz){
	m_mesh = 0;
	m_lightSource = 0;
	pos_x = posx;
	pos_y = posy;
	pos_z = posz;
	rot_x = 0.0f;
	rot_y = 0.0f;
	rot_z = 0.0f;
	point_pos_x = 0.0f;
	point_pos_y = 0.0f;
	point_pos_z = 0.0f;
	scale_x = 0.25f;
	scale_y = 0.25f;
	scale_z = 0.25f;
	m_rotationSpeed_y = 0.0025f;
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
	//Release the vertex and index buffers
	ModelClass::ShutdownBuffers();

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

	rot_y += m_rotationSpeed_y;
	if (rot_y > 2 * (float) XM_PI)
		rot_y = rot_y - 2*(float) XM_PI;

	CalculateWorldMatrix();
	return true;
}