#pragma once

//////////////
// INCLUDES //

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //

class PlanetClass :
	public ModelClass
{
public:
	PlanetClass();
	~PlanetClass();

	bool Initialize(MeshClass* mesh, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);
private:
	virtual bool PreProcessing();

	XMFLOAT3 m_rotVel;
};

