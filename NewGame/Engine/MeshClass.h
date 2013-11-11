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
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normals;
	};

	struct VertexMaterialType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normals;
	};

	struct VertexTextType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

public:
	MeshClass() {};
	~MeshClass() {};
	bool Initialize(string filename, MeshType type);
	bool Initialize(ObjectMeshClass *object, MeshType type);
	void Shutdown();


	MeshType getMeshType();
	vector<ObjectMeshClass*>* getAllObjects();
	vector<DirectX::XMFLOAT3>* getGuns();

private:
	bool LoadModel(string filename);
	void ReleaseModel();
	bool InitializeBuffers();
	void ShutdownBuffers();

	vector<ObjectMeshClass*> m_allObjects;
	vector<DirectX::XMFLOAT3> m_guns;
	MeshType m_type;
};