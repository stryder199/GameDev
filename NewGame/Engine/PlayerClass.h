//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class MeshClass;
class ShaderControllerClass;
class D3DClass;
class CameraClass;
class TextureClass;
class LightClass;

//=================================
// included dependencies
#include "ModelClass.h"

using namespace DirectX;

class PlayerClass : public ModelClass{
public:
	PlayerClass();
	~PlayerClass();

	bool Initialize(MeshClass* mesh);
	void Shutdown();

	bool Render(ShaderControllerClass* shader, LightClass* lightSource);

	void SetVelX(float x);
	void SetVelY(float y);
	void SetVelZ(float z);
	void SetRotVelX(float x);
	void SetRotVelY(float y);
	void SetRotVelZ(float z);
private:
	virtual bool PreProcessing();

	float vel_x, vel_y, vel_z;
	float rotVel_x, rotVel_y, rotVel_z;
};