#include "MeshControllerClass.h"
#include "MeshClass.h"

using namespace std;

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
    m_allMeshs = map<string, shared_ptr<MeshClass>>();
}

void MeshControllerClass::Shutdown()
{
    map<string, shared_ptr<MeshClass>>::iterator mesh;
    for (mesh = m_allMeshs.begin(); mesh != m_allMeshs.end(); ++mesh)
    {
        (*mesh).second->Shutdown();
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

    shared_ptr<MeshClass> newMesh(new MeshClass());
    newMesh->Initialize(filename, type);

    meshMutex.lock();
    m_allMeshs[name] = newMesh;
    meshMutex.unlock();
}

shared_ptr<MeshClass> MeshControllerClass::getMesh(string name)
{
    meshMutex.lock();
    if (m_allMeshs.find(name) == m_allMeshs.end())
    {
        return nullptr;
    }

    shared_ptr<MeshClass> myMesh = m_allMeshs[name];
    meshMutex.unlock();
    return myMesh;
}