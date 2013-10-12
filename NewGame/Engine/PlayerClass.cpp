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
	m_vertexBuffer = 0;
	m_colorBuffer = 0;
	m_indexBuffer = 0;
	pos_x = 0.0f;
	pos_y = 0.0f;
	pos_z = 0.0f;
	vel_x = 0.0f;
	vel_y = 0.0f;
	vel_z = 0.0f;
}

PlayerClass::~PlayerClass(){
}

bool PlayerClass::Initialize( MeshClass* objMesh, TextureClass* objTex, ID3D11Device* device )
{
	bool result;

	m_mesh = objMesh;
	m_texture = objTex;

	//Initialize the vertex and index buffers that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if(!result)
		return false;

	return true;
}

void PlayerClass::Shutdown()
{
	//Release the vertex and index buffers
	ShutdownBuffers();

	return;
}

bool PlayerClass::Render(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource)
{
	bool result;

	result = RenderBuffers(D3D, shader, camera, lightSource);
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

bool PlayerClass::RenderBuffers(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource)
{
	unsigned int vertexstride;
	unsigned int offset;
	bool result;
	XMFLOAT4X4 projMatrix;

	//Set vertex buffer stride and offset.
	if (m_mesh->getMeshColorType() == MeshClass::MeshColorType::MATERIAL)
	{
		vertexstride = sizeof(VertexMaterialType);
	}
	else if (m_mesh->getMeshColorType() == MeshClass::MeshColorType::TEXTURE)
	{
		vertexstride = sizeof(VertexTextureType);
	}
	offset = 0;

	result = PreProcessing();
	if(!result)
		return false;

	//Set the vertex buffer to active in the input assembler so it can be rendered.
	D3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexstride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered.
	D3D->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	D3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D->GetProjectionMatrix(projMatrix);

	//Render the model using the shader
	result = shader->Render(D3D->GetDeviceContext(), m_mesh->getIndexCount(), m_worldMatrix, *camera->GetViewMatrix(), projMatrix,
		m_texture->GetTexture(), lightSource->GetDirection(), lightSource->GetAmbientColor(), lightSource->GetDiffuseColor());
	if(!result)
		return false;

	return true;
}