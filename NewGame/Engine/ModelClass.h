//=================================
// include guard
#pragma once

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
	virtual bool Render(ShaderControllerClass* shader) = 0;
	virtual void Shutdown() = 0;

	float getPositionX();
	float getPositionY();
	float getPositionZ();
	float getRotationX();
	float getRotationY();
	float getRotationZ();
	float getScaleX();
	float getScaleY();
	float getScaleZ();
	float getPointPositionX();
	float getPointPositionY();
	float getPointPositionZ();

protected:
	bool InitializeBuffers();
	bool RenderBuffers(ShaderControllerClass* shader);
	void ShutdownBuffers();
	void CalculateWorldMatrix();
	virtual bool PreProcessing() = 0;

	MeshClass* m_mesh;
	LightClass *m_lightSource;
	float pos_x, pos_y, pos_z;
	float point_pos_x, point_pos_y, point_pos_z;
	float rot_x, rot_y, rot_z;
	float scale_x, scale_y, scale_z;
	XMFLOAT4X4 m_worldMatrix;
};