//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class ActorsClass;
class EnvironmentArtClass;
class ShaderControllerClass;
class LightClass;

//=================================
// included dependencies
#include <vector>
#include <D3D11.h>

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