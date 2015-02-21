#pragma once

//////////////
// INCLUDES //
#include <vector>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class MeshDataClass;

class ObjectMeshClass
{
public:
    ObjectMeshClass();
    ~ObjectMeshClass();

    void Initialize(std::string name);
    void Shutdown();

    void addMesh(MeshDataClass* newMesh);
    std::string getName();

    std::vector<MeshDataClass*>* getAllMeshData();
private:
    std::vector<MeshDataClass*> m_allMeshData;
    std::string m_name;
};

