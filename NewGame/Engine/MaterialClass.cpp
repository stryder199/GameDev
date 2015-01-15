#include "MaterialClass.h"


MaterialClass::MaterialClass()
{
}


MaterialClass::~MaterialClass()
{
}

bool MaterialClass::Initialize(MaterialClass::MaterialInfo matInfo)
{
	m_materialInfo = matInfo;
	
	return true;
}

void MaterialClass::Shutdown()
{
	return;
}

MaterialClass::MaterialInfo MaterialClass::getMaterialInfo()
{
	return m_materialInfo;
}