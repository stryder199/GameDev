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

bool MeshDataClass::Initialize(MeshColorType type, string textureFilename)
{
	bool result;

	m_type = type;

	m_vecMesh = vector<MeshType>();

	if (m_type == MATERIAL)
	{
		m_material = new MaterialClass();
		result = m_material->Initialize();
		if (!result)
			return false;
	}
	else if (m_type == TEXTURE)
	{
		m_texture = new TextureClass();
		result = m_texture->Initialize(toWChar(textureFilename));
		if (!result)
			return false;
	}

	return true;
}

void Shutdown()
{

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