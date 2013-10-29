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

	m_vecColors = vector<ColorType>();

	m_materialInfo = matInfo;
	
	return true;
}

void MaterialClass::Shutdown()
{

}
vector<MaterialClass::ColorType>* MaterialClass::getColors()
{
	return &m_vecColors;
}

void MaterialClass::addColorData(MaterialClass::ColorType data)
{
	m_vecColors.push_back(data);
}