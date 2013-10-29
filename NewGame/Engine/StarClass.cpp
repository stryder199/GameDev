#include "StarClass.h"
#include "MeshClass.h"

StarClass::StarClass(){
	m_mesh = 0;
	pos_x = 0.0f;
	pos_y = 0.0f;
	pos_z = -100.0f;
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

bool StarClass::Render(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource)
{
	bool result;

	result = PreProcessing();
	if (!result)
		return false;

	result = ModelClass::RenderBuffers(shader, camera, lightSource);
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