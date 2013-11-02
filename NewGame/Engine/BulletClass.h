#pragma once

class Timer;

#include <math.h>
#include "ModelClass.h"

using namespace DirectX;

class BulletClass :
	public ModelClass
{
public:
	BulletClass();
	~BulletClass();

	bool Initialize(MeshClass* mesh, XMFLOAT3 pos, XMFLOAT3 dir);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);

	int GetTimeAlive();
private:
	virtual bool PreProcessing();
	Timer* m_life;
	XMFLOAT3 m_vel;
};

