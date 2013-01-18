//=================================
// include guard
#ifndef Unit_H_
#define Unit_H_

//=================================
// forward declared dependencies
class Device;
class Timer;
class Bullet;
class Model;

//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <vector>

class Unit
{

private:

	//Replace with a Constant Buffer??
	XMMATRIX* g_World;

	Model *mesh;

	Timer *unitTimer;
	Timer *shootingIntervalTimer;

	std::vector<Bullet*> *bulletContainer;

	float xVel;
	float yVel;
	float zVel;

	bool shooting;

public:
	Unit( Device *myDevice );
	XMMATRIX* getMatrix();
	void setMatrix( XMMATRIX *var );
	void updateVars( Device *myDevice );
	void calculatePos();
	void eventHandle( MSG *msg );
	void handleShooting( Device *myDevice );
	void spawnBullet( Device *myDevice );

};

#endif /* Unit_H_ */