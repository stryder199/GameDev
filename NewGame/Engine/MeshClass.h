////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

class ObjectMeshClass;

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: MeshClass
////////////////////////////////////////////////////////////////////////////////
class MeshClass
{

public:
	MeshClass();
	MeshClass(const MeshClass&);
	~MeshClass();

	bool Initialize(char* filename);
	void Shutdown();

	vector<ObjectMeshClass*>* getAllObjects();

private:
	bool LoadModel(char* filename);
	void ReleaseModel();

	vector<ObjectMeshClass*> m_allObjects;
	int m_totalVertexCount, m_totalIndexCount;
};

#endif