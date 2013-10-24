//=================================
// include guard
#ifndef ModelClass_H_
#define ModelClass_H_

//=================================
// forward declared dependencies
class MeshClass;
class TextureClass;
class D3DClass;
class ShaderControllerClass;
class CameraClass;
class LightClass;

//=================================
// included dependencies
#include <D3D11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class ModelClass{
protected:
	struct VertexTextureType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normals;
	};

	struct VertexMaterialType
	{
		XMFLOAT3 position;
		XMFLOAT3 normals;
		XMFLOAT4 color;
	};

public:
	virtual bool Initialize(MeshClass* mesh) = 0;
	virtual bool Render(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource) = 0;
	virtual void Shutdown() = 0;

protected:
	bool InitializeBuffers();
	bool RenderBuffers(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource);
	void ShutdownBuffers();
	virtual bool PreProcessing() = 0;

	MeshClass* m_mesh;
	float pos_x, pos_y, pos_z;
	XMFLOAT4X4 m_worldMatrix;
};

#endif /* ModelClass_H_ */