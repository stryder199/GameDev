////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MeshClass.h"
#include "MaterialClass.h"

MeshClass::MeshClass()
{
	m_mesh = 0;
}

MeshClass::MeshClass(const MeshClass& other)
{
}

MeshClass::~MeshClass()
{
	m_material = 0;
}

bool MeshClass::Initialize(char* meshFilename, MeshColorType type)
{
	bool result;

	m_type = type;

	if (m_type == MATERIAL)
	{
		m_material = new MaterialClass();
	}

	// Load in the model data,
	result = LoadModel(meshFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void MeshClass::Shutdown()
{
	ReleaseModel();

	return;
}

int MeshClass::getIndexCount()
{
	return m_indexCount;
}

int MeshClass::getVertexCount()
{
	return m_vertexCount;
}

MeshClass::MeshType* MeshClass::getMesh()
{
	return m_mesh;
}

MeshClass::MeshColorType MeshClass::getMeshColorType()
{
	return m_type;
}

MaterialClass* MeshClass::getMaterial()
{
	return m_material;
}

bool MeshClass::LoadModel(char* filename)
{
	std::ifstream fin;
	std::string sinput;
	MaterialClass::MaterialInfo colorInfo;
	bool result;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	if (m_type == MATERIAL)
	{
		result = m_material->Initialize(m_indexCount);
		if (!result)
			return false;
	}

	// Create the model using the vertex count that was read in.
	m_mesh = new MeshType[m_vertexCount];
	if(!m_mesh)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> sinput;

		if (sinput.compare("mtl") == 0)
		{
			fin >> colorInfo.Ns >> colorInfo.Ka_r >> colorInfo.Ka_g >> colorInfo.Ka_b >> colorInfo.Kd_r >>
				colorInfo.Kd_g >> colorInfo.Kd_b >> colorInfo.Ks_r >> colorInfo.Ks_g >> colorInfo.Ks_b >>
				colorInfo.Ni >> colorInfo.d >> colorInfo.illum;
			i--;
		}
		else if (sinput.compare("vtn") == 0)
		{
			if (m_type == MATERIAL)
			{
				MaterialClass::ColorType color;
				color.r = colorInfo.Kd_r;
				color.g = colorInfo.Kd_g;
				color.b = colorInfo.Kd_b;
				color.a = 1.0f;

				m_material->getColors()[i] = color;
			}

			fin >> m_mesh[i].x >> m_mesh[i].y >> m_mesh[i].z;
			fin >> m_mesh[i].tu >> m_mesh[i].tv;
			fin >> m_mesh[i].nx >> m_mesh[i].ny >> m_mesh[i].nz;
		}		
	}

	// Close the model file.
	fin.close();

	return true;
}


void MeshClass::ReleaseModel()
{
	if(m_mesh)
	{
		delete [] m_mesh;
		m_mesh = 0;
	}

	return;
}