////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MeshClass.h"

MeshClass::MeshClass()
{
	m_mesh = 0;
}

MeshClass::MeshClass(const MeshClass& other)
{
}

MeshClass::~MeshClass()
{
}

bool MeshClass::Initialize(char* meshFilename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(meshFilename);
	if(!result)
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

bool MeshClass::LoadModel(char* filename)
{
	std::ifstream fin;
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
		fin >> m_mesh[i].x >> m_mesh[i].y >> m_mesh[i].z;
		fin >> m_mesh[i].tu >> m_mesh[i].tv;
		fin >> m_mesh[i].nx >> m_mesh[i].ny >> m_mesh[i].nz;
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