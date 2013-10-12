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
#include <D3D11.h>
#include <DirectXMath.h>
#include "ModelClass.h"

using namespace DirectX;

class PlayerClass : public ModelClass{
public:
	PlayerClass();
	~PlayerClass();

	bool Initialize(MeshClass* mesh, TextureClass* texture, ID3D11Device* device);
	bool Render(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource);
	void Shutdown();

private:
	bool RenderBuffers(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource);
	bool PreProcessing();

	float vel_x, vel_y, vel_z;
	XMFLOAT4X4 m_worldMatrix;

};

#endif /* PlayerClass_H_ */