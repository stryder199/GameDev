#pragma once

#include <map>
#include <vector>

#include <DirectXMath.h>
#include <rapidxml\rapidxml.hpp>

class XmlHelpers
{
public:
    static std::map<std::string, rapidxml::xml_node<>*>* GetNodeNameMap(rapidxml::xml_node<>*);
    static std::vector<rapidxml::xml_node<>*>* GetNodeVector(rapidxml::xml_node<>* root);
    static DirectX::XMFLOAT2 GetNodeVector2(rapidxml::xml_node<>* root);
    static DirectX::XMFLOAT3 GetNodeVector3(rapidxml::xml_node<>* root);
    static DirectX::XMFLOAT4 GetNodeVector4(rapidxml::xml_node<>* root);
};

