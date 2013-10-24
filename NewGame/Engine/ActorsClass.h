//=================================
// include guard
#ifndef ActorsClass_H_
#define ActorsClass_H_

//=================================
// forward declared dependencies
class MeshClass;
class ShaderControllerClass;
class D3DClass;
class CameraClass;
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
	bool RenderAll(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource);
	
private:
	std::vector<ModelClass*>* allModels;
	PlayerClass* player;
	MeshClass* playerMesh;
	TextureClass* playerTex;

};

#endif /* ActorsClass_H_ */