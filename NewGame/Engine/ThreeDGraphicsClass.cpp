#include "ThreeDGraphicsClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderControllerClass.h"
#include "LightClass.h"
#include "ModelClass.h"
#include "StarClass.h"
#include "PlanetClass.h"
#include "PlayerClass.h"
#include "MeshControllerClass.h"
#include "WindowClass.h"

ThreeDGraphicsClass::ThreeDGraphicsClass()
{
}

ThreeDGraphicsClass::~ThreeDGraphicsClass()
{
}

bool ThreeDGraphicsClass::Initialize()
{
	//bool result;
	m_allModels = std::vector<ModelClass*>();

	return true;
}

bool ThreeDGraphicsClass::RenderAll(ShaderControllerClass* shader){
	bool result;

	ConstructFrustum();

	modelMutex.lock();
	vector<ModelClass*>::iterator model;
	for (model = m_allModels.begin(); model != m_allModels.end(); ++model)
	{
		if (CheckSphereAgainstFrustum((*model)->getPosition(), (*model)->getBasicCollisionCircleRadius()))
		{
			result = (*model)->Render(shader);
			if (!result)
				return false;
		}
	}
	modelMutex.unlock();

	return true;
}

bool ThreeDGraphicsClass::AddPlayer(string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos)
{
	bool result;
	PlayerClass *player = PlayerClass::getInstance();
	MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);
	if (mesh == NULL)
		return false;

	result = player->Initialize(mesh, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos);
	if (!result)
		return false;

	modelMutex.lock();
	m_allModels.push_back(player);
	modelMutex.unlock();

	return true;
}

bool ThreeDGraphicsClass::AddStar(string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel)
{
	bool result;
	StarClass *star = new StarClass();
	MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);
	if (mesh == NULL)
		return false;

	result = star->Initialize(mesh, pos, scale, rotVel);
	if (!result)
		return false;

	modelMutex.lock();
	m_allModels.push_back(star);
	modelMutex.unlock();

	return true;
}

bool ThreeDGraphicsClass::AddPlanet(string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel)
{
	bool result;
	PlanetClass *planet = new PlanetClass();
	MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);
	if (mesh == NULL)
		return false;

	result = planet->Initialize(mesh, pos, scale, rotVel);
	if (!result)
		return false;

	modelMutex.lock();
	m_allModels.push_back(planet);
	modelMutex.unlock();

	return true;
}

void ThreeDGraphicsClass::ConstructFrustum()
{
	float zMinimum, r;
	DirectX::XMMATRIX matrix;
	DirectX::XMFLOAT4X4 matrixFloat;
	DirectX::XMVECTOR normalized;

	DirectX::XMFLOAT4X4 projectionFloat4x4 = D3DClass::getInstance()->GetProjectionMatrix();
	DirectX::XMFLOAT4X4 viewFloat4x4 = CameraClass::getInstance()->GetViewMatrix();
	float screenDepth = WindowClass::getInstance()->SCREEN_DEPTH;

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionFloat4x4._43 / projectionFloat4x4._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionFloat4x4._33 = r;
	projectionFloat4x4._43 = -r * zMinimum;
	DirectX::XMMATRIX projMat = DirectX::XMLoadFloat4x4(&projectionFloat4x4);
	DirectX::XMMATRIX viewMat = DirectX::XMLoadFloat4x4(&viewFloat4x4);

	// Create the frustum matrix from the view matrix and updated projection matrix.
	matrix = DirectX::XMMatrixMultiply(viewMat, projMat);

	DirectX::XMStoreFloat4x4(&matrixFloat, matrix);

	// Calculate near plane of frustum.
	m_fulstrumPlanes[0].x = matrixFloat._14 + matrixFloat._13;
	m_fulstrumPlanes[0].y = matrixFloat._24 + matrixFloat._23;
	m_fulstrumPlanes[0].z = matrixFloat._34 + matrixFloat._33;
	m_fulstrumPlanes[0].w = matrixFloat._44 + matrixFloat._43;
	DirectX::XMVECTOR plane0 = DirectX::XMLoadFloat4(&m_fulstrumPlanes[0]);
	normalized = DirectX::XMPlaneNormalize(plane0);
	DirectX::XMStoreFloat4(&m_fulstrumPlanes[0], normalized);

	// Calculate far plane of frustum.
	m_fulstrumPlanes[1].x = matrixFloat._14 - matrixFloat._13;
	m_fulstrumPlanes[1].y = matrixFloat._24 - matrixFloat._23;
	m_fulstrumPlanes[1].z = matrixFloat._34 - matrixFloat._33;
	m_fulstrumPlanes[1].w = matrixFloat._44 - matrixFloat._43;
	DirectX::XMVECTOR plane1 = DirectX::XMLoadFloat4(&m_fulstrumPlanes[1]);
	normalized = DirectX::XMPlaneNormalize(plane1);
	DirectX::XMStoreFloat4(&m_fulstrumPlanes[1], normalized);

	// Calculate left plane of frustum.
	m_fulstrumPlanes[2].x = matrixFloat._14 + matrixFloat._11;
	m_fulstrumPlanes[2].y = matrixFloat._24 + matrixFloat._21;
	m_fulstrumPlanes[2].z = matrixFloat._34 + matrixFloat._31;
	m_fulstrumPlanes[2].w = matrixFloat._44 + matrixFloat._41;
	DirectX::XMVECTOR plane2 = DirectX::XMLoadFloat4(&m_fulstrumPlanes[2]);
	normalized = DirectX::XMPlaneNormalize(plane2);
	DirectX::XMStoreFloat4(&m_fulstrumPlanes[2], normalized);

	// Calculate right plane of frustum.
	m_fulstrumPlanes[3].x = matrixFloat._14 - matrixFloat._11;
	m_fulstrumPlanes[3].y = matrixFloat._24 - matrixFloat._21;
	m_fulstrumPlanes[3].z = matrixFloat._34 - matrixFloat._31;
	m_fulstrumPlanes[3].w = matrixFloat._44 - matrixFloat._41;
	DirectX::XMVECTOR plane3 = DirectX::XMLoadFloat4(&m_fulstrumPlanes[3]);
	normalized = DirectX::XMPlaneNormalize(plane3);
	DirectX::XMStoreFloat4(&m_fulstrumPlanes[3], normalized);

	// Calculate top plane of frustum.
	m_fulstrumPlanes[4].x = matrixFloat._14 - matrixFloat._12;
	m_fulstrumPlanes[4].y = matrixFloat._24 - matrixFloat._22;
	m_fulstrumPlanes[4].z = matrixFloat._34 - matrixFloat._32;
	m_fulstrumPlanes[4].w = matrixFloat._44 - matrixFloat._42;
	DirectX::XMVECTOR plane4 = DirectX::XMLoadFloat4(&m_fulstrumPlanes[4]);
	normalized = DirectX::XMPlaneNormalize(plane4);
	DirectX::XMStoreFloat4(&m_fulstrumPlanes[4], normalized);

	// Calculate bottom plane of frustum.
	m_fulstrumPlanes[5].x = matrixFloat._14 + matrixFloat._12;
	m_fulstrumPlanes[5].y = matrixFloat._24 + matrixFloat._22;
	m_fulstrumPlanes[5].z = matrixFloat._34 + matrixFloat._32;
	m_fulstrumPlanes[5].w = matrixFloat._44 + matrixFloat._42;
	DirectX::XMVECTOR plane5 = DirectX::XMLoadFloat4(&m_fulstrumPlanes[5]);
	normalized = DirectX::XMPlaneNormalize(plane5);
	DirectX::XMStoreFloat4(&m_fulstrumPlanes[5], normalized);

	return;
}

bool ThreeDGraphicsClass::CheckSphereAgainstFrustum(DirectX::XMFLOAT3 pos, float radius)
{
	int i;
	DirectX::XMVECTOR posVec = DirectX::XMLoadFloat3(&pos);

	// Check if the radius of the sphere is inside the view frustum.
	for (i = 0; i<6; i++)
	{
		DirectX::XMVECTOR planeVec = DirectX::XMLoadFloat4(&m_fulstrumPlanes[i]);
		float result = 0.0f;
		DirectX::XMStoreFloat(&result, DirectX::XMPlaneDotCoord(planeVec, posVec));
		if (result < -radius)
		{
			return false;
		}
	}

	return true;
}