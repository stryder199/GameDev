#pragma once

//////////////
// INCLUDES //
#include <map>

///////////////////////
// MY CLASS INCLUDES //
#include "MeshClass.h"

/////////////////
// FORWARD DEC //

using namespace std;

class MeshControllerClass
{
public:
	~MeshControllerClass();

	bool Initialize();

	static MeshControllerClass* getInstance();

	bool addObjMesh(string filename, string name, MeshClass::MeshType type);
	bool addPrimitiveMesh(string filename, string name, MeshClass::MeshType type);
	MeshClass* getMesh(string name);
private:
	MeshControllerClass();
	MeshControllerClass(const MeshControllerClass&) {};
	MeshControllerClass& operator=(const MeshControllerClass&) {};

	static MeshControllerClass* m_pInstance;

	// name, mesh
	map<string, MeshClass*> m_allMeshs;
};

