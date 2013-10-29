#include "EnvironmentArtClass.h"
#include "PlanetClass.h"
#include "MeshClass.h"
#include "StarClass.h"
#include "ModelClass.h"

EnvironmentArtClass::EnvironmentArtClass()
{
}

EnvironmentArtClass::~EnvironmentArtClass()
{
}

bool EnvironmentArtClass::Initialize()
{
	bool result;

	allStars = vector<StarClass*>();

	allPlanets = vector<PlanetClass*>();

	MeshClass *earthMesh = new MeshClass();
	result = earthMesh->Initialize("data/earth.3dmodel");
	if (!result)
		return false;

	MeshClass *sunMesh = new MeshClass();
	result = sunMesh->Initialize("data/sun.3dmodel");
	if (!result)
		return false;

	PlanetClass* earth = new PlanetClass();
	result = earth->Initialize(earthMesh);
	if (!result)
		return false;

	allPlanets.push_back(earth);

	StarClass* sun = new StarClass();
	result = sun->Initialize(sunMesh);
	if (!result)
		return false;

	allStars.push_back(sun);

	return true;
}

bool EnvironmentArtClass::RenderAll(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource)
{
	vector<StarClass*>::iterator starIt;
	vector<PlanetClass*>::iterator planetIt;

	for (starIt = allStars.begin(); starIt != allStars.end(); ++starIt)
	{
		(*starIt)->Render(shader, camera, lightSource);
	}

	for (planetIt = allPlanets.begin(); planetIt != allPlanets.end(); ++planetIt)
	{
		(*planetIt)->Render(shader, camera, lightSource);
	}

	return true;
}