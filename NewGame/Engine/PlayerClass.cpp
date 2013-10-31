#include "PlayerClass.h"
#include "ModelClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"

PlayerClass::PlayerClass(){
	m_mesh = 0;
	m_lightSource = 0;
	pos_x = 0.0f;
	pos_y = 0.0f;
	pos_z = 0.0f;
	rot_x = 0.0f;
	rot_y = 0.0f;
	rot_z = 0.0f;
	point_pos_x = 0.0f;
	point_pos_y = 0.0f;
	point_pos_z = 0.0f;
	scale_x = 0.008f;
	scale_y = 0.008f;
	scale_z = 0.008f;
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

	return true;
}

void PlayerClass::Shutdown()
{
	//Release the vertex and index buffers
	ModelClass::ShutdownBuffers();

	return;
}

bool PlayerClass::Render(ShaderControllerClass* shader)
{
	bool result;

	result = PreProcessing();
	if (!result)
		return false;

	//Generate the view matrix based on the camera's position
	CameraClass::getInstance()->Render(this);

	result = ModelClass::RenderBuffers(shader);
	if(!result)
		return false;

	return true;
}

bool PlayerClass::PreProcessing()
{
	//bool result; not used
	float maxThrust = 0.01f;

	rot_x += rotVel_x;
	if (rot_x >= 2 * (float) XM_PI)
		rot_x = rot_x - 2 * (float) XM_PI;
	else if (rot_x < 0.0f)
		rot_x = 2 * (float) XM_PI + rot_x;

	rot_y += rotVel_y;
	if (rot_y >= 2 * (float) XM_PI)
		rot_y = rot_y - 2 * (float) XM_PI;
	else if (rot_y < 0.0f)
		rot_y = 2 * (float) XM_PI + rot_y;

	rot_z += rotVel_z;
	if (rot_z >= 2 * (float) XM_PI)
		rot_z = rot_z - 2 * (float) XM_PI;
	else if (rot_z < 0.0f)
		rot_z = 2 * (float) XM_PI + rot_z;

	if (engine_power == 1.0f)
	{
		if (rot_y >= 0.0f && rot_y < (XM_PI/2.0f))
		{
			vel_x = (rot_y / (XM_PI / 2.0f)) * maxThrust;
		}
		else if (rot_y >= (XM_PI / 2.0f) && rot_y < ((3.0f*XM_PI) / 2.0f))
		{
			vel_x = ((XM_PI - rot_y) / (XM_PI / 2.0f)) * maxThrust;
		}
		else if (rot_y >= ((3.0f*XM_PI) / 2.0f) && rot_y < (2 * XM_PI))
		{
			vel_x = ((rot_y - 2*XM_PI) / (XM_PI / 2.0f)) * maxThrust;
		}

		if (rot_y >= 0.0f && rot_y < XM_PI)
		{
			vel_z = (((XM_PI / 2.0f) - rot_y) / (XM_PI / 2.0f)) * maxThrust;
		}
		else if (rot_y >= XM_PI && rot_y < (2 * XM_PI))
		{
			vel_z = ((rot_y - ((3.0f*XM_PI) / 2.0f)) / (XM_PI / 2.0f)) * maxThrust;
		}

		vel_y = 0.0f;
	}
	else if (engine_power == -1.0f)
	{
		if (rot_y >= 0.0f && rot_y < (XM_PI / 2.0f))
		{
			vel_x = (rot_y / (XM_PI / 2.0f)) * maxThrust;
		}
		else if (rot_y >= (XM_PI / 2.0f) && rot_y < ((3.0f*XM_PI) / 2.0f))
		{
			vel_x = ((XM_PI - rot_y) / (XM_PI / 2.0f)) * maxThrust;
		}
		else if (rot_y >= ((3.0f*XM_PI) / 2.0f) && rot_y < (2 * XM_PI))
		{
			vel_x = ((rot_y - 2 * XM_PI) / (XM_PI / 2.0f)) * maxThrust;
		}

		if (rot_y >= 0.0f && rot_y < XM_PI)
		{
			vel_z = (((XM_PI / 2.0f) - rot_y) / (XM_PI / 2.0f)) * maxThrust;
		}
		else if (rot_y >= XM_PI && rot_y < (2 * XM_PI))
		{
			vel_z = ((rot_y - ((3.0f*XM_PI) / 2.0f)) / (XM_PI / 2.0f)) * maxThrust;
		}

		vel_x *= -1.0f;
		vel_z *= -1.0f;
		vel_y = 0.0f;
	}
	else
	{
		vel_x = 0.0f;
		vel_y = 0.0f;
		vel_z = 0.0f;
	}

	pos_x += vel_x;
	pos_y += vel_y;
	pos_z += vel_z;

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

void PlayerClass::SetEnginePower(float power)
{
	engine_power = power;
}