#pragma once

//////////////
// INCLUDES //
#include <D3D11.h>
#include <DirectXMath.h>
#include <vector>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class MeshClass;
class ShaderControllerClass;
class LightClass;

using namespace DirectX;

class ModelClass{
public:
	virtual bool Render(ShaderControllerClass* shader) = 0;
	virtual void Shutdown() = 0;

	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale();
	XMFLOAT3 getPointPosition();
	XMFLOAT3 getDirection();

protected:
	bool RenderBuffers(ShaderControllerClass* shader);
	void CalculateWorldMatrix();
	void ConstrainRotation();
	void CalculateDirection();
	virtual bool PreProcessing() = 0;

	MeshClass* m_mesh;
	LightClass *m_lightSource;
	XMFLOAT3 m_pos, m_point_pos, m_rot, m_scale, m_dir;
	XMFLOAT4 m_color;
	XMFLOAT4X4 m_worldMatrix;
};