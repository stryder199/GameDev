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
	};

	struct VertexTextType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	virtual bool Render(ShaderControllerClass* shader) = 0;
	virtual void Shutdown() = 0;

	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale();
	XMFLOAT3 getPointPosition();
	XMFLOAT3 getDirection();

protected:
	bool InitializeBuffers();
	bool RenderBuffers(ShaderControllerClass* shader);
	void ShutdownBuffers();
	void CalculateWorldMatrix();
	void ConstrainRotation();
	void CalculateDirection();
	virtual bool PreProcessing() = 0;

	MeshClass* m_mesh;
	LightClass *m_lightSource;
	XMFLOAT3 m_pos, m_point_pos, m_rot, m_scale, m_dir;
	XMFLOAT4X4 m_worldMatrix;
};