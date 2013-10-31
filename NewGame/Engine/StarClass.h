//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class MeshClass;

//=================================
// included dependencies
#include "ModelClass.h"

class StarClass : ModelClass{

public:
	StarClass(float posx, float posy, float posz);
	~StarClass();
	
	bool Initialize(MeshClass* mesh);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);
private:
	virtual bool PreProcessing();

	float m_rotationSpeed_y;

};