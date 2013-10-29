//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class ActorsClass;
class EnvironmentArtClass;
class ShaderControllerClass;
class D3DClass;
class LightClass;
class CameraClass;

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

	bool RenderAll(ShaderControllerClass* shader, CameraClass* camera);

	ActorsClass* getActors();

private:
	EnvironmentArtClass* m_envArt;
	ActorsClass* m_actors;
	LightClass* m_lightSource;
};