//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class MeshClass;
class ShaderControllerClass;
class LightClass;
class TextureClass;

//=================================
// included dependencies
#include <vector>
#include <D3D11.h>
#include "PlayerClass.h"

class ActorsClass{

public:
	ActorsClass();
	~ActorsClass();

	bool Initialize();
	bool RenderAll(ShaderControllerClass* shader);
private:
	std::vector<ModelClass*> allModels;

};