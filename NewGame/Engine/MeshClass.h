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

	struct VertexTextureType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normals;
	};

	struct VertexMaterialType
	{
		XMFLOAT3 position;
		XMFLOAT3 normals;
	};

	struct VertexTextType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	MeshClass() {};
	~MeshClass() {};
    void Initialize(string filename, MeshType type);
    void Initialize(ObjectMeshClass *object, MeshType type);
	void Shutdown();


	MeshType getMeshType();
	vector<ObjectMeshClass*>* getAllObjects();
	vector<XMFLOAT3>* getGuns();

private:
    void LoadModel(string filename);
    void InitializeBuffers();

	vector<ObjectMeshClass*> m_allObjects;
	vector<XMFLOAT3> m_guns;
	MeshType m_type;
};