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
	modelMutex.lock();
	std::vector<ModelClass*>::iterator it;
	for (it = m_allModels.begin(); it != m_allModels.end(); ++it)
	{
		if ((*it) != NULL)
		{
			result = (*it)->Render(shader);
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