#pragma once

//////////////////////
///Includes
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

//////////////////////
///Forward Dec
class Vector3;
class Vector4;

using namespace std;

class MeshClass
{
public:

	typedef struct{
		float x, y, z;
	}VertexType;

	typedef struct
	{
		float tu, tv;
	}TextureData;

	typedef struct
	{
		int v, t, n;
	}FaceType;

	typedef struct
	{
		float r, g, b, a;
	}ColorType;

	MeshClass();
	~MeshClass(void);

	bool Initialize(string filename, Vector3 *color);
	void Shutdown();

	int getVertexCount();
	VertexType* getVertices();
	TextureData* getTextures();
	VertexType* getNormals();
	unsigned int* getIndices();
	VertexType* getColors();
	VertexType* getTangents();
	VertexType* getBinomials();
	void getFace(int index, Vector4 *v1, Vector4 *v2, Vector4 *v3, Vector3 *normal);
	void resetColor(Vector3 newColor);

private:
	bool loadMesh(string filename, VertexType color);
	bool ReadFileCounts(string filename);
	bool LoadDataStructures(string filename, VertexType color);
	void CalculateTangentsBinomials(int vIndex1, int vIndex2, int vIndex3, Vector3& tangent, Vector3& binormal);
	void MeshClass::CalculateNormal(Vector3 tangent, Vector3 binormal, Vector3& normal);

	vector<VertexType>* mVertices;
	vector<TextureData>* mTexturesCord;
	vector<VertexType>* mNormals;
	vector<unsigned int>*mIndices;
	vector<VertexType>* mColors;
	vector<VertexType>* mTangents;
	vector<VertexType>* mBinomials;

	int mVertexCount;
};
