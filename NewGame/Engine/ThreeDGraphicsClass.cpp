#include "ThreeDGraphicsClassh.h"
#include "EnvironmentArtClass.h"
#include "ActorsClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderControllerClass.h"
#include "LightClass.h"

ThreeDGraphicsClass::ThreeDGraphicsClass()
{
	m_envArt = 0;
	m_actors = 0;
}

ThreeDGraphicsClass::ThreeDGraphicsClass(const ThreeDGraphicsClass& other)
{
}

ThreeDGraphicsClass::~ThreeDGraphicsClass()
{
}

bool ThreeDGraphicsClass::Initialize()
{
	bool result;

	m_envArt = new EnvironmentArtClass();
	result = m_envArt->Initialize();
	if(!result)
		return false;

	m_actors = new ActorsClass();
	result = m_actors->Initialize();
	if(!result)
		return false;

	return true;
}

bool ThreeDGraphicsClass::RenderAll(ShaderControllerClass* shader)
{
	bool result;

	shader->Set3DMaterialShaders();

	result = m_envArt->RenderAll(shader);
	if(!result)
		return false;

	result = m_actors->RenderAll(shader);
	if(!result)
		return false;

	return true;
}

ActorsClass* ThreeDGraphicsClass::getActors()
{
	return m_actors;
}