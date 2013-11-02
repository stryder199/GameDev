#include "CameraClass.h"
#include "PlayerClass.h"

CameraClass* CameraClass::m_pInstance = NULL;

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
	if (!m_pInstance)
		m_pInstance = new CameraClass();

	return m_pInstance;
}

bool CameraClass::Initialize()
{
	//bool result; not used

	return true;
}

void CameraClass::Render(PlayerClass* player)
{
	XMVECTOR up, position, lookAt;
	XMFLOAT3 playerOrigin;
	XMMATRIX positionRotMatrixX, positionRotMatrixY, positionRotMatrixZ;
	XMMATRIX worldMatrix;
	XMMATRIX pointTranslationMatrix, translationMatrix;

	playerOrigin = player->getPosition();
	
	m_rot = player->getRotation();
	m_rot.y += XM_PI;

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

XMFLOAT4X4* CameraClass::GetViewMatrix()
{
	return m_viewMatrix;
}