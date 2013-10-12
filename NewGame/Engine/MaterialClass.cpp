#include "MaterialClass.h"


MaterialClass::MaterialClass()
{
	m_colorCount = 0;
	m_Colors = 0;
}


MaterialClass::~MaterialClass()
{
}

bool MaterialClass::Initialize(int colorCount)
{
	bool result;
	m_colorCount = colorCount;

	m_Colors = new MaterialClass::ColorType[m_colorCount];
	
	return true;
}

void MaterialClass::Shutdown()
{

}

MaterialClass::ColorType* MaterialClass::getColors()
{
	return m_Colors;
}

int MaterialClass::getColorCount()
{
	return m_colorCount;
}