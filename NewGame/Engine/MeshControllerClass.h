#pragma once

//////////////
// INCLUDES //
#include <map>
#include <mutex>

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

    void Initialize();
	void Shutdown();

	static MeshControllerClass* getInstance();

    void addMesh(string filename, string name, MeshClass::MeshType type);
	MeshClass* getMesh(string name);
private:
	MeshControllerClass();
	MeshControllerClass(const MeshControllerClass&) {};
	MeshControllerClass& operator=(const MeshControllerClass&) {};

	static MeshControllerClass* m_pInstance;
	static mutex instanceMutex;

	// name, mesh
	mutex meshMutex;
	map<string, MeshClass*> m_allMeshs;
};

