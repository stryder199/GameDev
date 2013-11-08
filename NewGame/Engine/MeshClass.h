#pragma once

//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class ObjectMeshClass;

using namespace std;

class MeshClass
{
public:
	enum MeshType
	{
		THREED = 0,
		TWOD = 1,
		TEXT = 2,
		NONE = 3
	};

public:
	friend class MeshControllerClass;

	virtual void Shutdown() = 0;

	MeshType getMeshType();
	vector<ObjectMeshClass*>* getAllObjects();
	vector<DirectX::XMFLOAT3>* getGuns();

private:
	bool Initialize(string filename, MeshType type);
	bool Initialize(ObjectMeshClass *object, MeshType type);
	void Shutdown();

	bool LoadModel(string filename);
	void ReleaseModel();

	vector<ObjectMeshClass*> m_allObjects;
	vector<DirectX::XMFLOAT3> m_guns;
	MeshType m_type;
};