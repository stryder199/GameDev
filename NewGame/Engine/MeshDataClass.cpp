#include "MeshDataClass.h"
#include "MaterialClass.h"
#include "TextureClass.h"


MeshDataClass::MeshDataClass()
{
	m_material = 0;
	m_texture = 0;
	m_indexCount = 0;
	m_vertexCount = 0;
	m_colorBuffer = 0;
	m_indexBuffer = 0;
	m_vertexBuffer = 0;
}


MeshDataClass::~MeshDataClass()
{
}

void MeshDataClass::Initialize(MaterialClass* mat)
{
	m_colorType = MATERIAL;

	m_vecMesh = vector<MeshDataType>();

	m_material = mat;
}

void MeshDataClass::Initialize(TextureClass* tex)
{
	m_colorType = TEXTURE;

	m_vecMesh = vector<MeshDataType>();

	m_texture = tex;
}

void MeshDataClass::Shutdown()
{
	if (m_material)
	{
		m_material->Shutdown();
		delete m_material;
		m_material = 0;
	}
	if (m_texture)
	{
		m_texture->Shutdown();
	}
	if (m_colorBuffer)
	{
		m_colorBuffer->Release();
		m_colorBuffer = 0;
	}
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

vector<MeshDataClass::MeshDataType>* MeshDataClass::getRawMeshData()
{
	return &m_vecMesh;
}

int MeshDataClass::getIndexCount()
{
	return m_vecMesh.size();
}

int MeshDataClass::getVertexCount()
{
	return m_vecMesh.size();
}

MeshDataClass::MeshColorType MeshDataClass::getMeshColorType()
{
	return m_colorType;
}

MaterialClass* MeshDataClass::getMaterial()
{
	return m_material;
}

ID3D11Buffer* MeshDataClass::getColorBuffer()
{
	return m_colorBuffer;
}

ID3D11Buffer* MeshDataClass::getVertexBuffer()
{
	return m_vertexBuffer;
}

ID3D11Buffer* MeshDataClass::getIndexBuffer()
{
	return m_indexBuffer;
}

TextureClass* MeshDataClass::getTexture()
{
	return m_texture;
}

void MeshDataClass::addMeshData(MeshDataType data)
{
	m_vecMesh.push_back(data);
}

void MeshDataClass::setVertexBuffer(ID3D11Buffer* buf)
{
	m_vertexBuffer = buf;
}

void MeshDataClass::setIndexBuffer(ID3D11Buffer* buf)
{
	m_indexBuffer = buf;
}

void MeshDataClass::setColorBuffer(ID3D11Buffer* buf)
{
	m_colorBuffer = buf;
}