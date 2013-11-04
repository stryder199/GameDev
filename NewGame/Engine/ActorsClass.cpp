#include "ActorsClass.h"
#include "PlayerClass.h"
#include "MeshClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"
#include "TextureClass.h"

ActorsClass::ActorsClass()
{
	m_player = 0;
}

ActorsClass::~ActorsClass(){
}

bool ActorsClass::Initialize()
{
	bool result;

	allModels = vector<ModelClass*>();

	MeshClass* playerMesh = new MeshClass();
	result = playerMesh->Initialize("data/Ghost_Fighter.3dmodel", MeshClass::THREED);
	if(!result)
		return false;

	m_player = new PlayerClass();
	result = m_player->PlayerClass::Initialize(playerMesh);
	if(!result)
		return false;

	allModels.push_back(m_player);

	return true;
}

bool ActorsClass::RenderAll(ShaderControllerClass* shader){
	bool result;
	std::vector<ModelClass*>::iterator it;
	
	for (it = allModels.begin(); it != allModels.end(); ++it)
	{
		result = (*it)->Render(shader);
		if(!result)
			return false;
	}

	return true;
}

PlayerClass* ActorsClass::getPlayer()
{
	return m_player;
}