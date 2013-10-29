#include "MeshDataClass.h"
#include "MaterialClass.h"
#include "TextureClass.h"


MeshDataClass::MeshDataClass()
{
	m_material = 0;
	m_texture = 0;
	m_indexCount = 0;
	m_vertexCount = 0;
}


MeshDataClass::~MeshDataClass()
{
}

bool MeshDataClass::Initialize(MaterialClass* mat)
{
	//bool result; not used

	m_type = MATERIAL;

	m_vecMesh = vector<MeshType>();

	m_material = mat;

	return true;
}

bool MeshDataClass::Initialize(TextureClass* tex)
{
	//bool result; not used

	m_type = TEXTURE;

	m_vecMesh = vector<MeshType>();

	m_texture = tex;

	return true;
}

void MeshDataClass::Shutdown()
{
	return;
}

vector<MeshDataClass::MeshType>* MeshDataClass::getRawMeshData()
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
	return m_type;
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

void MeshDataClass::addMeshData(MeshType data)
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