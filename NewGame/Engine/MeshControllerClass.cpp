#include "MeshControllerClass.h"
#include "MeshClass.h"

MeshControllerClass* MeshControllerClass::m_pInstance = NULL;
std::mutex MeshControllerClass::instanceMutex;

MeshControllerClass::MeshControllerClass()
{
}


MeshControllerClass::~MeshControllerClass()
{
}

bool MeshControllerClass::Initialize()
{
	//bool result;
	
	m_allMeshs = map<string, MeshClass*>();

	return true;
}

MeshControllerClass* MeshControllerClass::getInstance()
{
	instanceMutex.lock();
	if (!m_pInstance)
		m_pInstance = new MeshControllerClass();
	instanceMutex.unlock();

	return m_pInstance;
}

bool MeshControllerClass::addMesh(string filename, string name, MeshClass::MeshType type)
{
	bool result;

	meshMutex.lock();
	if (m_allMeshs.find(name) != m_allMeshs.end())
	{
		// Already in the list
		return true;
	}
	meshMutex.unlock();

	MeshClass *newMesh = 0;
	newMesh = new MeshClass();
	if (!newMesh)
		return false;
	result = newMesh->Initialize(filename, type);
	if (!result)
		return false;

	meshMutex.lock();
	m_allMeshs[name] = newMesh;
	meshMutex.unlock();

	return true;
}

MeshClass* MeshControllerClass::getMesh(string name)
{
	meshMutex.lock();
	if (m_allMeshs.find(name) == m_allMeshs.end())
	{
		return NULL;
	}

	MeshClass* myMesh = m_allMeshs[name];
	meshMutex.unlock();
	return myMesh;
}