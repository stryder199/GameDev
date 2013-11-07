#pragma once

//////////////
// INCLUDES //
#include <vector>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class StarClass;
class PlanetClass;
class ShaderControllerClass;

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