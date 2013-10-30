#include "PlayerClass.h"
#include "ModelClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"

PlayerClass::PlayerClass(){
	m_mesh = 0;
	pos_x = 0.0f;
	pos_y = 0.0f;
	pos_z = 0.0f;
	rot_x = 0.0f;
	rot_y = 0.0f;
	rot_z = 0.0f;
	point_pos_x = 0.0f;
	point_pos_y = 0.0f;
	point_pos_z = 0.0f;
	scale_x = 0.002f;
	scale_y = 0.002f;
	scale_z = 0.002f;
	vel_x = 0.0f;
	vel_y = 0.0f;
	vel_z = 0.0f;
	rotVel_x = 0.0f;
	rotVel_y = 0.0f;
	rotVel_z = 0.0f;
}

PlayerClass::~PlayerClass(){
}

bool PlayerClass::Initialize( MeshClass* objMesh )
{
	bool result;

	m_mesh = objMesh;

	//Initialize the vertex and index buffers that hold the geometry for the triangle.
	result = ModelClass::InitializeBuffers();
	if(!result)
		return false;

	return true;
}

void PlayerClass::Shutdown()
{
	//Release the vertex and index buffers
	ModelClass::ShutdownBuffers();

	return;
}

bool PlayerClass::Render(ShaderControllerClass* shader, LightClass* lightSource)
{
	bool result;

	result = PreProcessing();
	if (!result)
		return false;

	//Generate the view matrix based on the camera's position
	CameraClass::getInstance()->Render(this);

	result = ModelClass::RenderBuffers(shader, lightSource);
	if(!result)
		return false;

	return true;
}

bool PlayerClass::PreProcessing()
{
	//bool result; not used

	pos_x += vel_x;
	pos_y += vel_y;
	pos_z += vel_z;

	rot_x += rotVel_x;
	if (rot_x > 2 * (float) XM_PI)
		rot_x = rot_x - 2 * (float) XM_PI;

	rot_y += rotVel_y;
	if (rot_y > 2 * (float) XM_PI)
		rot_y = rot_y - 2 * (float) XM_PI;

	rot_z += rotVel_z;
	if (rot_z > 2 * (float) XM_PI)
		rot_z = rot_z - 2 * (float) XM_PI;

	CalculateWorldMatrix();
	return true;
}

void PlayerClass::SetVelX(float x)
{
	vel_x = x;
}

void PlayerClass::SetVelY(float y)
{
	vel_y = y;
}

void PlayerClass::SetVelZ(float z)
{
	vel_z = z;
}

void PlayerClass::SetRotVelX(float x)
{
	rotVel_x = x;
}
void PlayerClass::SetRotVelY(float y)
{
	rotVel_y = y;
}

void PlayerClass::SetRotVelZ(float z)
{
	rotVel_z = z;
}