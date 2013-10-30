#include "PlanetClass.h"
#include "ModelClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"

PlanetClass::PlanetClass()
{
	m_mesh = 0;
	m_rotationSpeed_y = 0.0025f;
	pos_x = 0.0f;
	pos_y = 0.0f;
	pos_z = 10.0f;
	rot_x = 0.366519143f;
	rot_y = 0.0f;
	rot_z = 0.0f;
	point_pos_x = 0.0f;
	point_pos_y = 0.0f;
	point_pos_z = 0.0f;
	scale_x = 0.025f;
	scale_y = 0.025f;
	scale_z = 0.025f;
}


PlanetClass::~PlanetClass()
{
}

bool PlanetClass::Initialize(MeshClass* objMesh)
{
	bool result;

	m_mesh = objMesh;

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

bool PlanetClass::Render(ShaderControllerClass* shader, LightClass* lightSource)
{
	bool result;

	result = PreProcessing();
	if (!result)
		return false;

	result = ModelClass::RenderBuffers(shader, lightSource);
	if (!result)
		return false;

	return true;
}

bool PlanetClass::PreProcessing()
{
	//bool result; not used

	rot_y += m_rotationSpeed_y;
	if (rot_y > 2*(float) XM_PI)
		rot_y = rot_y - 2*(float) XM_PI;

	CalculateWorldMatrix();
	return true;
}