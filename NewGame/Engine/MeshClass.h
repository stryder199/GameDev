////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <DirectXMath.h>

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
	vector<DirectX::XMFLOAT3>* getGuns();

private:
	bool LoadModel(char* filename);
	void ReleaseModel();

	vector<ObjectMeshClass*> m_allObjects;
	vector<DirectX::XMFLOAT3> m_guns;
};