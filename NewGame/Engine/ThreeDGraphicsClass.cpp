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

		// Create the light object.
	m_lightSource = new LightClass;
	if(!m_lightSource)
		return false;

	// Initialize the light object.
	m_lightSource->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightSource->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

bool ThreeDGraphicsClass::RenderAll(ShaderControllerClass* shader)
{
	bool result;

	shader->Set3DMaterialShaders();

	result = m_envArt->RenderAll(shader, m_lightSource);
	if(!result)
		return false;

	result = m_actors->RenderAll(shader, m_lightSource);
	if(!result)
		return false;

	return true;
}

ActorsClass* ThreeDGraphicsClass::getActors()
{
	return m_actors;
}