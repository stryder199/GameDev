////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "BitmapClass.h"
#include "D3DClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "PlayerClass.h"

BitmapClass::BitmapClass()
{
	m_mesh = 0;
	m_lightSource = 0;
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


BitmapClass::BitmapClass(const BitmapClass& other)
{
}


BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(MeshClass* mesh, LightClass *lightSource, XMFLOAT2 pos, XMFLOAT2 scale)
{
	bool result;

	m_mesh = mesh;
	m_lightSource = lightSource;
	m_pos = XMFLOAT3(pos.x, pos.y, 0.0f);
	m_scale = XMFLOAT3(scale.x, scale.y, 0.0f);

	result = ModelClass::InitializeBuffers();
	if (!result)
		return false;

	return true;
}

void BitmapClass::Shutdown()
{
	ModelClass::ShutdownBuffers();
	return;
}

bool BitmapClass::Render(ShaderControllerClass *shader)
{
	bool result;

	//Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen
	result = PreProcessing();
	if(!result)
		return false;

	ModelClass::RenderBuffers(shader);

	return true;
}

bool BitmapClass::PreProcessing()
{
	m_rot = CameraClass::getInstance()->getRotation();
	m_pos = PlayerClass::getInstance()->getPosition();

	ConstrainRotation();

	CalculateWorldMatrix();

	return true;
}