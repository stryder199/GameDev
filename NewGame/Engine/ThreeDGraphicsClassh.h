#pragma once

//////////////
// INCLUDES //
#include <vector>
#include <D3D11.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class ActorsClass;
class EnvironmentArtClass;
class ShaderControllerClass;

using namespace std;

class ThreeDGraphicsClass{
public:
	ThreeDGraphicsClass();
	ThreeDGraphicsClass(const ThreeDGraphicsClass&);
	~ThreeDGraphicsClass();

	bool Initialize();

	bool RenderAll(ShaderControllerClass* shader);

	ActorsClass* getActors();

private:
	EnvironmentArtClass* m_envArt;
	ActorsClass* m_actors;
};