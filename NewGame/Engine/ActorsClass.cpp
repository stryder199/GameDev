#include "ActorsClass.h"
#include "PlayerClass.h"
#include "MeshClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"
#include "TextureClass.h"

ActorsClass::ActorsClass(){
	playerMesh = 0;
	playerTex = 0;
}

ActorsClass::~ActorsClass(){
}

bool ActorsClass::Initialize()
{
	bool result;

	playerMesh = new MeshClass();
	result = playerMesh->Initialize("data/ship1a.3dmodel");
	if(!result)
		return false;

	allModels = new std::vector<ModelClass*>;

	player = new PlayerClass();
	result = player->PlayerClass::Initialize(playerMesh);
	if(!result)
		return false;
	allModels->push_back(player);

	return true;
}

bool ActorsClass::RenderAll(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource){
	bool result;
	std::vector<ModelClass*>::iterator it;
	
	for (it = allModels->begin(); it != allModels->end(); ++it)
	{
		result = (*it)->Render(shader, camera, lightSource);
		if(!result)
			return false;
	}

	return true;
}