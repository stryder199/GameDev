#include "MeshControllerClass.h"
#include "MeshClass.h"
#include "ObjMeshClass.h"
#include "PrimitiveMeshClass.h"

MeshControllerClass* MeshControllerClass::m_pInstance = NULL;

MeshControllerClass::MeshControllerClass()
{
}


MeshControllerClass::~MeshControllerClass()
{
}

bool MeshControllerClass::Initialize()
{
	m_allMeshs = map<string, MeshClass*>();
}

MeshControllerClass* MeshControllerClass::getInstance()
{
	if (!m_pInstance)
		m_pInstance = new MeshControllerClass();

	return m_pInstance;
}

bool MeshControllerClass::addPrimitiveMesh(string filename, string name, MeshClass::MeshType type)
{
	bool result;

	if (m_allMeshs.find(name) != m_allMeshs.end())
	{
		// Already in the list
		return true;
	}
	MeshClass *newMesh = 0;
	newMesh = new PrimitiveMeshClass();
	if (!newMesh)
		return false;
	result = newMesh->Initialize(filename, type);
	if (!result)
		return false;

	m_allMeshs[name] = newMesh;

	return true;
}

bool MeshControllerClass::addObjMesh(string filename, string name, MeshClass::MeshType type)
{
	bool result;

	if (m_allMeshs.find(name) != m_allMeshs.end())
	{
		// Already in the list
		return true;
	}
	MeshClass *newMesh = 0;
	newMesh = new ObjMeshClass();
	if (!newMesh)
		return false;
	result = newMesh->Initialize(filename, type);
	if (!result)
		return false;

	m_allMeshs[name] = newMesh;

	return true;
}

MeshClass* MeshControllerClass::getMesh(string name)
{
	if (m_allMeshs.find(name) == m_allMeshs.end())
	{
		return NULL;
	}
	
	return m_allMeshs[name];
}