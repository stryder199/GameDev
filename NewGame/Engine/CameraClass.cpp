#include "CameraClass.h"
#include "PlayerClass.h"

CameraClass::CameraClass()
{
	m_viewMatrix = 0;

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_velX = 0.0f;
	m_velY = 0.0f;
	m_velZ = 0.0f;

	m_velRotX = 0.0f;
	m_velRotY = 0.0f;
	m_velRotZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

bool CameraClass::Initialize()
{
	//bool result; not used

	m_viewMatrix = new XMFLOAT4X4();

	//Set the initial position of the camera
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);

	return true;
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	return;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::setVelX( float vel )
{
	m_velX = vel;
}

void CameraClass::setVelY( float vel )
{
	m_velY = vel;
}

void CameraClass::setVelZ( float vel )
{
	m_velZ = vel;
}

void CameraClass::setVelRotX( float vel )
{
	m_velRotX = vel;
}

void CameraClass::setVelRotY( float vel )
{
	m_velRotY = vel;
}

void CameraClass::setVelRotZ( float vel )
{
	m_velRotZ = vel;
}

void CameraClass::Render(PlayerClass* player)
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	//Setup the vector that points upwards
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_positionX = player->getPositionX();
	m_positionY = player->getPositionY();
	m_positionZ = player->getPositionZ() + -2.0f;

	//Setup the position of the camera in the world
	position = XMVectorSet(m_positionX, m_positionY, m_positionZ, 0.0f);

	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	m_rotationX = player->getRotationX();
	m_rotationY = player->getRotationY();
	m_rotationZ = player->getRotationZ();

	//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) roations in radians;
	pitch = m_rotationX;
	yaw   = m_rotationY;
	roll  = m_rotationZ ;

	//Create the rotation matrix from the yaw, pitch and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//Transform the lookAt and up vector by the rotation matrux so the view is correctly rotated at the origin
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	//Translate the rotated camera position to the location of the viewer
	lookAt = position + lookAt;

	//Finally create the view matrix from the three updates vectors
	XMStoreFloat4x4(m_viewMatrix, XMMatrixLookAtLH(position, lookAt, up));
}

XMFLOAT4X4* CameraClass::GetViewMatrix()
{
	return m_viewMatrix;
}