#pragma once

//////////////
//Includes
#include <math.h>

/////////////////
///Forward Dec
class Vector3;
class Matrix4x4;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	Vector3 getPosition();

	void Render();
	Matrix4x4 GetViewMatrix();

private:
	void MatrixRotationYawPitchRoll(float*, float, float, float);
	void TransformCoord(Vector3&, float*);
	void BuildViewMatrix(Vector3, Vector3, Vector3);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	Matrix4x4* mViewMatrix;
};