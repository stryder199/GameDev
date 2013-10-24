#include "PlayerClass.h"
#include "ModelClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "D3DClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"
#include "TextureClass.h"
#include "MaterialClass.h"

PlayerClass::PlayerClass(){
	m_mesh = 0;
	pos_x = 0.0f;
	pos_y = 0.0f;
	pos_z = 0.0f;
	vel_x = 0.0f;
	vel_y = 0.0f;
	vel_z = 0.0f;
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

bool PlayerClass::Render(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource)
{
	bool result;

	result = PreProcessing();
	if (!result)
		return false;

	result = ModelClass::RenderBuffers(shader, camera, lightSource);
	if(!result)
		return false;

	return true;
}

bool PlayerClass::PreProcessing()
{
	bool result;

	pos_x += vel_x;
	pos_y += vel_y;
	pos_z += vel_z;

	XMMATRIX worldMatrix = XMMatrixIdentity();

	// Move the model to the location it should be rendered at.
	XMMATRIX translationMatrix = XMMatrixTranslation(pos_x, pos_y, pos_z);

	XMMATRIX scalingMatrix = XMMatrixScaling(0.25f, 0.25f, 0.25f);

	worldMatrix *= translationMatrix;
	worldMatrix *= scalingMatrix;

	XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
	return true;
}