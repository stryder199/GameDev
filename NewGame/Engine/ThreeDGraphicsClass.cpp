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

ThreeDGraphicsClass::ThreeDGraphicsClass(const ThreeDGraphicsClass& other)
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
	std::vector<ModelClass*>::iterator it;

	for (it = m_allModels.begin(); it != m_allModels.end(); ++it)
	{
		result = (*it)->Render(shader);
		if (!result)
			return false;
	}

	return true;
}

bool ThreeDGraphicsClass::AddPlayer(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos)
{
	bool result;
	PlayerClass *player = PlayerClass::getInstance();
	MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);
	if (mesh == NULL)
		return false;

	result = player->Initialize(mesh, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos);
	if (!result)
		return false;
	m_allModels.push_back(player);

	return true;
}

bool ThreeDGraphicsClass::AddStar(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel)
{
	bool result;
	StarClass *star = new StarClass();
	MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);
	if (mesh == NULL)
		return false;

	result = star->Initialize(mesh, pos, scale, rotVel);
	if (!result)
		return false;
	m_allModels.push_back(star);

	return true;
}

bool ThreeDGraphicsClass::AddPlanet(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel)
{
	bool result;
	PlanetClass *planet = new PlanetClass();
	MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);
	if (mesh == NULL)
		return false;

	result = planet->Initialize(mesh, pos, scale, rotVel);
	if (!result)
		return false;
	m_allModels.push_back(planet);

	return true;
}