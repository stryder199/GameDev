#include "MaterialClass.h"


MaterialClass::MaterialClass()
{
}


MaterialClass::~MaterialClass()
{
}

bool MaterialClass::Initialize(MaterialClass::MaterialInfo matInfo)
{
	//bool result; not used
	m_materialInfo = matInfo;
	
	return true;
}

void MaterialClass::Shutdown()
{

}

MaterialClass::MaterialInfo MaterialClass::getMaterialInfo()
{
	return m_materialInfo;
}