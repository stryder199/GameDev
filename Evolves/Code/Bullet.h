//=================================
// include guard
#ifndef Bullet_H_
#define Bullet_H_

//=================================
// forward declared dependencies
class Unit;
class Device;
class Timer;
class Model;

//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

class Bullet
{

private:
	XMMATRIX *g_World;
	Unit *owner;
	Timer *bulletTimer;
	Model *mesh;

	float xVel;
	float yVel;
	float zVel;

public:
	Bullet( Device *myDevice, Unit *ownerInit );
	XMMATRIX* getMatrix();
	void setMatrix( XMMATRIX *var );
	void calculatePos();
	void updateVars( Device *myDevice );

};

#endif /* Bullet_H_ */