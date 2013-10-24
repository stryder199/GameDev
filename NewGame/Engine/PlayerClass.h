//=================================
// include guard
#ifndef PlayerClass_H_
#define PlayerClass_H_

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
	bool Render(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource);
	void Shutdown();

private:
	bool RenderBuffers(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource);
	bool PreProcessing();

	float vel_x, vel_y, vel_z;
};

#endif /* PlayerClass_H_ */