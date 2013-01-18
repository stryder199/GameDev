#include "GameData.h"

GameData::GameData()
{
	allUnits = new std::vector<Unit*>();
}

GameData::~GameData()
{
	std::vector<Unit *>::iterator it;

	for ( it=allUnits->begin() ; it != allUnits->end(); it++ )
	{
		delete((*it));
	}
}

std::vector<Unit*>* GameData::getAllUnits()
{
	return allUnits;
}

int GameData::loadFiles()
{
	return 0;
}