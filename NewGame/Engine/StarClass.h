#pragma once

//////////////
// INCLUDES //

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //

class StarClass : 
	public ModelClass
{
public:
	StarClass();
	~StarClass();
	
	bool Initialize(MeshClass* objMesh, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);
	virtual bool PreProcessing();
private:
	

	XMFLOAT3 m_rotVel;
};