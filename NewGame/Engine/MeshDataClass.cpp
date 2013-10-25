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
	bool result;

	m_type = MATERIAL;

	m_vecMesh = vector<MeshType>();

	m_material = mat;

	return true;
}

bool MeshDataClass::Initialize(TextureClass* tex)
{
	bool result;

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
	return m_indexCount;
}

int MeshDataClass::getVertexCount()
{
	return m_vertexCount;
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

wchar_t* toWChar(string s)
{
	wchar_t* wide_string = new wchar_t[s.length() + 1];
	std::copy(s.begin(), s.end(), wide_string);
	wide_string[s.length()] = 0;
	return wide_string;
}