#include "EnvironmentArtClass.h"

EnvironmentArtClass::EnvironmentArtClass()
{
	allStars = 0;
}

EnvironmentArtClass::~EnvironmentArtClass()
{
}

bool EnvironmentArtClass::Initialize()
{
	allStars = new std::vector<StarClass>;

	return true;
}

bool EnvironmentArtClass::RenderAll(){


	return true;
}
