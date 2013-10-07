#include "ActorsClass.h"
#include "PlayerClass.h"
#include "MeshClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"
#include "TextureClass.h"

ActorsClass::ActorsClass(){
	player = 0;
	playerMesh = 0;
	playerTex = 0;
}

ActorsClass::~ActorsClass(){
}

bool ActorsClass::Initialize(ID3D11Device* device)
{
	bool result;

	playerMesh = new MeshClass();
	result = playerMesh->Initialize("../Engine/data/ship1.txt");
	if(!result)
		return false;

	// Create the texture object.
	playerTex = new TextureClass;

	// Initialize the texture object.
	result = playerTex->Initialize(device, L"../Engine/data/shiptex.dds");
	if(!result)
	{
		return false;
	}

	//allPlayers = new std::vector<PlayerClass>;

	player = new PlayerClass();
	result = player->Initialize(playerMesh, playerTex, device);
	if(!result)
		return false;

	return true;
}

bool ActorsClass::RenderAll(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource){
	bool result;

	result = player->Render(D3D, shader, camera, lightSource);
	if(!result)
		return false;

	return true;
}