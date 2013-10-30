#include "CameraClass.h"
#include "PlayerClass.h"

CameraClass* CameraClass::m_pInstance = NULL;

CameraClass::CameraClass()
{
	m_viewMatrix = 0;

	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;

	m_velX = 0.0f;
	m_velY = 0.0f;
	m_velZ = 0.0f;

	m_velRotX = 0.0f;
	m_velRotY = 0.0f;
	m_velRotZ = 0.0f;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;

	m_pointPosX = 0.0f;
	m_pointPosY = 0.0f;
	m_pointPosZ = -10.0f;
}

CameraClass* CameraClass::getInstance()
{
	if (!m_pInstance)
		m_pInstance = new CameraClass();

	return m_pInstance;
}

bool CameraClass::Initialize()
{
	//bool result; not used

	m_viewMatrix = new XMFLOAT4X4();

	return true;
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;

	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;

	return;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_posX, m_posY, m_posZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotX, m_rotY, m_rotZ);
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
	XMMATRIX worldMatrix;
	XMMATRIX pointTranslationMatrix;

	//Setup the vector that points upwards
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	worldMatrix = XMMatrixIdentity();

	m_posX = player->getPositionX();
	m_posY = player->getPositionY();
	m_posZ = player->getPositionZ();

	//Setup the position of the camera in the world
	position = XMVectorSet(m_posX, m_posY, m_posZ, 0.0f);
	
	m_rotX = player->getRotationX();
	m_rotY = player->getRotationY();
	m_rotZ = player->getRotationZ();

	//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) roations in radians;
	pitch = m_rotX;
	yaw   = m_rotY;
	roll  = m_rotZ;

	//Create the rotation matrix from the yaw, pitch and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	pointTranslationMatrix = XMMatrixTranslation(m_pointPosX, m_pointPosY, m_pointPosZ);

	//worldMatrix *= pointTranslationMatrix;
	worldMatrix *= rotationMatrix;

	//Transform the lookAt and up vector by the rotation matrux so the view is correctly rotated at the origin
	lookAt = XMVector3TransformCoord(lookAt, worldMatrix);
	up = XMVector3TransformCoord(up, worldMatrix);

	//Translate the rotated camera position to the location of the viewer
	lookAt = position + lookAt;

	//Finally create the view matrix from the three updates vectors
	XMStoreFloat4x4(m_viewMatrix, XMMatrixLookAtLH(position, lookAt, up));
}

XMFLOAT4X4* CameraClass::GetViewMatrix()
{
	return m_viewMatrix;
}