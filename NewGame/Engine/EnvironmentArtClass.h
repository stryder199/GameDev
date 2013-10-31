//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class StarClass;
class PlanetClass;
class ShaderControllerClass;
class CameraClass;
class LightClass;

//=================================
// included dependencies
#include <vector>

using namespace std;

class EnvironmentArtClass{

public:
	EnvironmentArtClass();
	~EnvironmentArtClass();
	
	bool Initialize();
	bool RenderAll(ShaderControllerClass* shader);

private:
	vector<StarClass*> allStars;
	vector<PlanetClass*> allPlanets;
};