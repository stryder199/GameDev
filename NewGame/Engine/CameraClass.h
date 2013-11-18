#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //

using namespace DirectX;

class CameraClass{
public:
	~CameraClass() {};
	static CameraClass* getInstance();

	bool Initialize();
	void Shutdown();
	
	void Render();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale();
	XMFLOAT3 getPointPosition();
	XMFLOAT3 getDirection();

	void ConstrainRotation();
	void CalculateDirection();
private:
	CameraClass();
	CameraClass(const CameraClass&) {};
	CameraClass& operator=(const CameraClass&) {};

	static CameraClass* m_pInstance;
	static std::mutex instanceMutex;

	XMFLOAT3 m_pos, m_point_pos, m_rot, m_scale, m_dir;
	XMFLOAT4X4 m_viewMatrix;

};