#include "ObjectMeshClass.h"
#include "MeshDataClass.h"


ObjectMeshClass::ObjectMeshClass()
{
	m_name = "";
}


ObjectMeshClass::~ObjectMeshClass()
{
}

bool ObjectMeshClass::Initialize(string name)
{
	bool result;

	m_name = name;
	m_allMeshData = vector<MeshDataClass*>();

	return true;
}

void ObjectMeshClass::Shutdown()
{
	vector<MeshDataClass*>::iterator it;
	for (it = m_allMeshData.begin(); it != m_allMeshData.end(); ++it)
	{
		(*it)->Shutdown();
	}

	return;
}

vector<MeshDataClass*>* ObjectMeshClass::getAllMeshData()
{
	return &m_allMeshData;
}

void ObjectMeshClass::addMesh(MeshDataClass* newMesh)
{
	m_allMeshData.push_back(newMesh);
}

string ObjectMeshClass::getName()
{
	return m_name;
}