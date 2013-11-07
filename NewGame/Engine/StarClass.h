#pragma once

//////////////
// INCLUDES //

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //

class StarClass : ModelClass{

public:
	StarClass(float posx, float posy, float posz);
	~StarClass();
	
	bool Initialize(MeshClass* mesh);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);
private:
	virtual bool PreProcessing();

	XMFLOAT3 m_rotVel;
};