#pragma once

//////////////
// INCLUDES //
#include <map>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //
#include "MeshClass.h"

/////////////////
// FORWARD DEC //

class MeshControllerClass
{
public:
    ~MeshControllerClass();

    void Initialize();
    void Shutdown();

    static MeshControllerClass* getInstance();

    void addMesh(std::string filename, std::string name, MeshClass::MeshType type);
    MeshClass* getMesh(std::string name);
private:
    MeshControllerClass();
    MeshControllerClass(const MeshControllerClass&) {};
    MeshControllerClass& operator=(const MeshControllerClass&) {};

    static MeshControllerClass* m_pInstance;
    static std::mutex instanceMutex;

    // name, mesh
    std::mutex meshMutex;
    std::map<std::string, MeshClass*> m_allMeshs;
};

