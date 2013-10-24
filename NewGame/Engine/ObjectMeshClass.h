#pragma once

//=================================
// forward declared dependencies
class MeshDataClass;

//=================================
// included dependencies
#include <vector>

using namespace std;

class ObjectMeshClass
{
public:
	ObjectMeshClass();
	~ObjectMeshClass();

	bool Initialize(string name);
	void Shutdown();

	void addMesh(MeshDataClass* newMesh);
	string getName();

	vector<MeshDataClass*>* getAllMeshData();
private:
	vector<MeshDataClass*> m_allMeshData;
	string m_name;
};

