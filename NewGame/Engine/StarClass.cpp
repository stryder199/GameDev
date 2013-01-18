#include "StarClass.h"
#include "MeshClass.h"

StarClass::StarClass(){
	mesh = 0;
}

StarClass::~StarClass(){
}

bool StarClass::Initialize()
{
	bool result;
	mesh = new MeshClass();
	//mesh->Initialize();

	return true;
}

bool StarClass::Render()
{
	bool result;

	return true;
}