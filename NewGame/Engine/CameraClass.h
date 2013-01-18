//=================================
// include guard
#ifndef CameraClass_H_
#define CameraClass_H_

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <D3DX10math.h>

class CameraClass{

public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	bool Initialize();
	D3DXMATRIX* GetViewMatrix();

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
	D3DXMATRIX* m_viewMatrix;

};

#endif /* CameraClass_H_ */