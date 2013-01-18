////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
// Class name: MeshClass
////////////////////////////////////////////////////////////////////////////////
class MeshClass
{
private:
	struct MeshType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	MeshClass();
	MeshClass(const MeshClass&);
	~MeshClass();

	bool Initialize(char*);
	void Shutdown();

	int getIndexCount();
	int getVertexCount();
	MeshType* getMesh();
	

private:
	bool LoadModel(char* filename);
	void ReleaseModel();

	int m_vertexCount, m_indexCount;
	MeshType* m_mesh;
};

#endif