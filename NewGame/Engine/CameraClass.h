//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class PlayerClass;

//=================================
// included dependencies
#include <DirectXMath.h>

using namespace DirectX;

class CameraClass{
public:
	~CameraClass();
	static CameraClass* getInstance();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render(PlayerClass* player);
	bool Initialize();
	XMFLOAT4X4* GetViewMatrix();

	void setVelX(float);
	void setVelY(float);
	void setVelZ(float);

	void setVelRotX(float);
	void setVelRotY(float);
	void setVelRotZ(float);

private:
	CameraClass();
	CameraClass(const CameraClass&) {};
	CameraClass& operator=(CameraClass& const) {};

	static CameraClass* m_pInstance;
	XMFLOAT3 m_pos, m_point_pos, m_rot, m_scale, m_dir;
	XMFLOAT4X4 *m_viewMatrix;

};