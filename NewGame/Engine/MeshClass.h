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

class MaterialClass;

////////////////////////////////////////////////////////////////////////////////
// Class name: MeshClass
////////////////////////////////////////////////////////////////////////////////
class MeshClass
{
public:
	struct MeshType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	enum MeshColorType
	{
		MATERIAL = 0,
		TEXTURE = 1,
		NONE = 2
	};

public:
	MeshClass();
	MeshClass(const MeshClass&);
	~MeshClass();

	bool Initialize(char* filename, MeshColorType type);
	void Shutdown();

	int getIndexCount();
	int getVertexCount();
	MeshType* getMesh();
	MeshColorType getMeshColorType();
	MaterialClass* getMaterial();
	

private:
	bool LoadModel(char* filename);
	void ReleaseModel();

	int m_vertexCount, m_indexCount;
	MeshType* m_mesh;
	MaterialClass* m_material;
	MeshColorType m_type;
};

#endif