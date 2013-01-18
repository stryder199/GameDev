//=================================
// include guard
#ifndef ActorsClass_H_
#define ActorsClass_H_

//=================================
// forward declared dependencies
class PlayerClass;
class MeshClass;
class ShaderClass;
class D3DClass;
class CameraClass;
class LightClass;
class TextureClass;

//=================================
// included dependencies
#include <vector>
#include <D3D11.h>

class ActorsClass{

public:
	ActorsClass();
	~ActorsClass();

	bool Initialize(ID3D11Device* device);
	bool RenderAll(D3DClass* D3D, ShaderClass* shader, CameraClass* camera, LightClass* lightSource);
	
private:
	//std::vector<PlayerClass>* allPlayers;
	PlayerClass* player;
	MeshClass* playerMesh;
	TextureClass* playerTex;

};

#endif /* ActorsClass_H_ */