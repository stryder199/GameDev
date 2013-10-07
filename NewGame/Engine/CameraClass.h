//=================================
// include guard
#ifndef CameraClass_H_
#define CameraClass_H_

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <DirectXMath.h>

using namespace DirectX;

class CameraClass{

public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	bool Initialize();
	XMMATRIX* GetViewMatrix();

	void setVelX(float);
	void setVelY(float);
	void setVelZ(float);

	void setVelRotX(float);
	void setVelRotY(float);
	void setVelRotZ(float);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_velX, m_velY, m_velZ;
	float m_velRotX, m_velRotY, m_velRotZ;
	XMMATRIX* m_viewMatrix;

};

#endif /* CameraClass_H_ */