#pragma once

#include "ModelClass.h"

class PlanetClass :
	public ModelClass
{
public:
	PlanetClass();
	~PlanetClass();

	bool Initialize(MeshClass* mesh);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);
private:
	virtual bool PreProcessing();

	XMFLOAT3 m_rotVel;
};

