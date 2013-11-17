#include "CameraClass.h"
#include "PlayerClass.h"

CameraClass* CameraClass::m_pInstance = NULL;
std::mutex CameraClass::instanceMutex;

CameraClass::CameraClass()
{
	m_viewMatrix = 0;

	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, -0.15f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_viewMatrix = new XMFLOAT4X4();
}

CameraClass* CameraClass::getInstance()
{
	instanceMutex.lock();
	if (!m_pInstance)
		m_pInstance = new CameraClass();
	instanceMutex.unlock();

	return m_pInstance;
}

bool CameraClass::Initialize()
{
	//bool result; not used

	return true;
}

void CameraClass::Render()
{
	XMVECTOR up, position, lookAt;
	XMFLOAT3 playerOrigin;
	XMMATRIX positionRotMatrixX, positionRotMatrixY, positionRotMatrixZ;
	XMMATRIX worldMatrix;
	XMMATRIX pointTranslationMatrix, translationMatrix;

	playerOrigin = PlayerClass::getInstance()->getPosition();
	
	m_rot = PlayerClass::getInstance()->getRotation();
	m_rot.y += XM_PI;

	ConstrainRotation();
	CalculateDirection();

	positionRotMatrixX = XMMatrixRotationX(m_rot.x);
	positionRotMatrixY = XMMatrixRotationY(m_rot.y);
	positionRotMatrixZ = XMMatrixRotationZ(m_rot.z);

	pointTranslationMatrix = XMMatrixTranslation(m_point_pos.x, m_point_pos.y, m_point_pos.z);
	translationMatrix = XMMatrixTranslation(playerOrigin.x, playerOrigin.y, playerOrigin.z);

	worldMatrix = XMMatrixIdentity();
	worldMatrix *= pointTranslationMatrix;
	worldMatrix *= positionRotMatrixX;
	worldMatrix *= positionRotMatrixY;
	worldMatrix *= positionRotMatrixZ;
	worldMatrix *= translationMatrix;

	position = XMVector4Transform(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), worldMatrix);

	//Setup the vector that points upwards
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	lookAt = XMVectorSet(playerOrigin.x, playerOrigin.y, playerOrigin.z, 0.0f) - position;
	lookAt = XMVector3Normalize(lookAt);

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, position);
	pos.y += 0.02;
	position = XMLoadFloat3(&pos);

	//Translate the rotated camera position to the location of the viewer
	lookAt = position + lookAt;

	XMStoreFloat3(&m_pos, position);
	//Finally create the view matrix from the three updates vectors
	XMStoreFloat4x4(m_viewMatrix, XMMatrixLookAtLH(position, lookAt, up));
}

XMFLOAT4X4 CameraClass::GetViewMatrix()
{
	return *m_viewMatrix;
}

XMFLOAT3 CameraClass::getPosition()
{
	return m_pos;
}

XMFLOAT3 CameraClass::getRotation()
{
	return m_rot;
}

XMFLOAT3 CameraClass::getScale()
{
	return m_scale;
}

XMFLOAT3 CameraClass::getPointPosition()
{
	return m_point_pos;
}

XMFLOAT3 CameraClass::getDirection()
{
	return m_dir;
}

void CameraClass::ConstrainRotation()
{
	if (m_rot.x >= 2 * (float) XM_PI)
		m_rot.x = m_rot.x - 2 * (float) XM_PI;
	else if (m_rot.x < 0.0f)
		m_rot.x = 2 * (float) XM_PI + m_rot.x;

	if (m_rot.y >= 2 * (float) XM_PI)
		m_rot.y = m_rot.y - 2 * (float) XM_PI;
	else if (m_rot.y < 0.0f)
		m_rot.y = 2 * (float) XM_PI + m_rot.y;

	if (m_rot.z >= 2 * (float) XM_PI)
		m_rot.z = m_rot.z - 2 * (float) XM_PI;
	else if (m_rot.z < 0.0f)
		m_rot.z = 2 * (float) XM_PI + m_rot.z;
}

void CameraClass::CalculateDirection()
{
	if (m_rot.y >= 0.0f && m_rot.y < (XM_PI / 2.0f))
	{
		m_dir.x = (m_rot.y / (XM_PI / 2.0f));
	}
	else if (m_rot.y >= (XM_PI / 2.0f) && m_rot.y < ((3.0f*XM_PI) / 2.0f))
	{
		m_dir.x = ((XM_PI - m_rot.y) / (XM_PI / 2.0f));
	}
	else if (m_rot.y >= ((3.0f*XM_PI) / 2.0f) && m_rot.y < (2 * XM_PI))
	{
		m_dir.x = ((m_rot.y - 2 * XM_PI) / (XM_PI / 2.0f));
	}

	if (m_rot.y >= 0.0f && m_rot.y < XM_PI)
	{
		m_dir.z = (((XM_PI / 2.0f) - m_rot.y) / (XM_PI / 2.0f));
	}
	else if (m_rot.y >= XM_PI && m_rot.y < (2 * XM_PI))
	{
		m_dir.z = ((m_rot.y - ((3.0f*XM_PI) / 2.0f)) / (XM_PI / 2.0f));
	}
	XMVECTOR normalizedVector;
	normalizedVector = XMVector3Normalize(XMLoadFloat3(&m_dir));
	XMStoreFloat3(&m_dir, normalizedVector);
}