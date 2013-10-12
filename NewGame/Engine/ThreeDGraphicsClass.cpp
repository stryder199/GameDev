#include "ThreeDGraphicsClassh.h"
#include "EnvironmentArtClass.h"
#include "ActorsClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderControllerClass.h"
#include "LightClass.h"

ThreeDGraphicsClass::ThreeDGraphicsClass()
{
	envArt = 0;
	actors = 0;
}

ThreeDGraphicsClass::ThreeDGraphicsClass(const ThreeDGraphicsClass& other)
{
}

ThreeDGraphicsClass::~ThreeDGraphicsClass()
{
}

bool ThreeDGraphicsClass::Initialize(ID3D11Device* device)
{
	bool result;

	envArt = new EnvironmentArtClass();
	result = envArt->Initialize();
	if(!result)
		return false;

	actors = new ActorsClass();
	result = actors->Initialize(device);
	if(!result)
		return false;

		// Create the light object.
	m_LightSource = new LightClass;
	if(!m_LightSource)
		return false;

	// Initialize the light object.
	m_LightSource->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_LightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightSource->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

bool ThreeDGraphicsClass::RenderAll(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera)
{
	bool result;

	shader->Set3DMaterialShaders();

	result = envArt->RenderAll();
	if(!result)
		return false;

	result = actors->RenderAll(D3D, shader, camera, m_LightSource);
	if(!result)
		return false;

	return true;
}