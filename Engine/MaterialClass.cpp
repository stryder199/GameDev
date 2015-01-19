#include "MaterialClass.h"


MaterialClass::MaterialClass()
{
}


MaterialClass::~MaterialClass()
{
}

void MaterialClass::Initialize(MaterialClass::MaterialInfo matInfo)
{
    m_materialInfo = matInfo;
}

void MaterialClass::Shutdown()
{
    return;
}

MaterialClass::MaterialInfo MaterialClass::getMaterialInfo()
{
    return m_materialInfo;
}