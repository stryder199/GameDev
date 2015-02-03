#include "MeshControllerClass.h"
#include "MeshClass.h"

MeshControllerClass* MeshControllerClass::m_pInstance = nullptr;
mutex MeshControllerClass::instanceMutex;

MeshControllerClass::MeshControllerClass()
{
}


MeshControllerClass::~MeshControllerClass()
{
}

void MeshControllerClass::Initialize()
{
    m_allMeshs = map<string, MeshClass*>();
}

void MeshControllerClass::Shutdown()
{
    map<string, MeshClass*>::iterator mesh;
    for (mesh = m_allMeshs.begin(); mesh != m_allMeshs.end(); ++mesh)
    {
        (*mesh).second->Shutdown();
        delete (*mesh).second;
        (*mesh).second = 0;
    }
}

MeshControllerClass* MeshControllerClass::getInstance()
{
    instanceMutex.lock();
    if (!m_pInstance)
        m_pInstance = new MeshControllerClass();
    instanceMutex.unlock();

    return m_pInstance;
}

void MeshControllerClass::addMesh(string filename, string name, MeshClass::MeshType type)
{
    meshMutex.lock();
    if (m_allMeshs.find(name) != m_allMeshs.end())
    {
        // Already in the list
        return;
    }
    meshMutex.unlock();

    MeshClass *newMesh = 0;
    newMesh = new MeshClass();
    newMesh->Initialize(filename, type);

    meshMutex.lock();
    m_allMeshs[name] = newMesh;
    meshMutex.unlock();
}

MeshClass* MeshControllerClass::getMesh(string name)
{
    meshMutex.lock();
    if (m_allMeshs.find(name) == m_allMeshs.end())
    {
        return nullptr;
    }

    MeshClass* myMesh = m_allMeshs[name];
    meshMutex.unlock();
    return myMesh;
}