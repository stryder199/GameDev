#pragma once

//////////////
// INCLUDES //
#include <math.h>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //
class Timer;

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

