#include "EnvironmentArtClass.h"
#include "PlanetClass.h"
#include "MeshClass.h"
#include "StarClass.h"
#include "ModelClass.h"
#include "WindowsHelpers.h"

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

	MeshClass *starMesh = new MeshClass();
	result = starMesh->Initialize("data/star.3dmodel");
	if (!result)
		return false;

	PlanetClass* earth = new PlanetClass();
	result = earth->Initialize(earthMesh);
	if (!result)
		return false;
	allPlanets.push_back(earth);

	StarClass* sun = new StarClass(0.0f, 0.0f, -50.0f);
	result = sun->Initialize(sunMesh);
	if (!result)
		return false;
	allStars.push_back(sun);

	// Generate some random stars
	for (int i = 0; i < 400; i++)
	{
		float posX = RandomFloat(0.0f, 200.0f);
		float posY = RandomFloat(0.0f, 200.0f);
		float posZ = RandomFloat(0.0f, 200.0f);

		if (posX < 100.f && posY < 100.0f && posZ < 100.0f)
		{
			int randomSeed = rand() % 3;
			if (randomSeed == 0)
			{
				posX += 100.0f;
			}
			else if (randomSeed == 1)
			{
				posY += 100.0f;
			}
			else if (randomSeed == 2)
			{
				posZ += 100.0f;
			}
		}

		if (int(posX) % 2 == 1)
			posX *= -1.0f;
		if (int(posY) % 2 == 1)
			posY *= -1.0f;
		if (int(posZ) % 2 == 1)
			posZ *= -1.0f;

		StarClass* newStar = new StarClass(posX, posY, posZ);
		result = newStar->Initialize(starMesh);
		if (!result)
			return false;
		allStars.push_back(newStar);
	}

	return true;
}

bool EnvironmentArtClass::RenderAll(ShaderControllerClass* shader)
{
	vector<StarClass*>::iterator starIt;
	vector<PlanetClass*>::iterator planetIt;

	for (starIt = allStars.begin(); starIt != allStars.end(); ++starIt)
	{
		(*starIt)->Render(shader);
	}

	for (planetIt = allPlanets.begin(); planetIt != allPlanets.end(); ++planetIt)
	{
		(*planetIt)->Render(shader);
	}

	return true;
}