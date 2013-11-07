#pragma once

//////////////
// INCLUDES //
#include <vector>
#include <D3D11.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class ModelClass;
class ShaderControllerClass;

using namespace std;

class ActorsClass{

public:
	ActorsClass();
	~ActorsClass();

	bool Initialize();
	bool RenderAll(ShaderControllerClass* shader);
private:
	vector<ModelClass*> allModels;

};