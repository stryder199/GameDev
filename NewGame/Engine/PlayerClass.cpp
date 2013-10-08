#include "PlayerClass.h"
#include "MeshClass.h"
#include "CameraClass.h"
#include "D3DClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"
#include "TextureClass.h"

PlayerClass::PlayerClass(){
	m_mesh = 0;
	m_vertexBuffer = 0;
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

void PlayerClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

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

bool PlayerClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_mesh->getVertexCount()];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_mesh->getIndexCount()];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(i=0; i<m_mesh->getVertexCount(); i++)
	{
		vertices[i].position = XMFLOAT3(m_mesh->getMesh()[i].x, m_mesh->getMesh()[i].y, m_mesh->getMesh()[i].z);
		vertices[i].texture = XMFLOAT2(m_mesh->getMesh()[i].tu, m_mesh->getMesh()[i].tv);
		vertices[i].normals = XMFLOAT3(m_mesh->getMesh()[i].nx, m_mesh->getMesh()[i].ny, m_mesh->getMesh()[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_mesh->getVertexCount();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_mesh->getIndexCount();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

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
	unsigned int stride;
	unsigned int offset;
	bool result;
	XMFLOAT4X4 projMatrix;

	//Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	result = PreProcessing();
	if(!result)
		return false;

	//Set the vertex buffer to active in the input assembler so it can be rendered.
	D3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

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