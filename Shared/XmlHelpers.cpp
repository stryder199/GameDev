#include "XmlHelpers.h"
#include "rapidxml\rapidxml.hpp"

#include <string>

using namespace DirectX;
using namespace std;
using namespace rapidxml;

vector<xml_node<>*>* XmlHelpers::GetNodeVector(xml_node<>* root)
{
    vector<xml_node<>*>* siblingNodes = new vector<xml_node<>*>();

    xml_node<>* node = root->first_node();
    if (node != nullptr)
    {
        siblingNodes->push_back(node);
    }

    while (node->next_sibling() != nullptr)
    {
        node = node->next_sibling();
        siblingNodes->push_back(node);
    }

    return siblingNodes;
}

map<string, xml_node<>*>* XmlHelpers::GetNodeNameMap(xml_node<>* root)
{
    map<string, xml_node<>*>* siblingNodes = new map<string, xml_node<>*>();

    xml_node<>* node = root->first_node();
    if (node != nullptr)
    {
        (*siblingNodes)[string(node->name())] = node;
    }

    while (node->next_sibling() != nullptr)
    {
        node = node->next_sibling();
        (*siblingNodes)[string(node->name())] = node;
    }

    return siblingNodes;
}

XMFLOAT2 XmlHelpers::GetNodeVector2(xml_node<>* root)
{
    map<string, xml_node<>*>* nodes = GetNodeNameMap(root);
    XMFLOAT2 data = XMFLOAT2();

    data.x = stof((*nodes)["x"]->value());
    data.y = stof((*nodes)["y"]->value());

    return data;
}

XMFLOAT3 XmlHelpers::GetNodeVector3(xml_node<>* root)
{
    map<string, xml_node<>*>* nodes = GetNodeNameMap(root);
    XMFLOAT3 data = XMFLOAT3();

    data.x = stof((*nodes)["x"]->value());
    data.y = stof((*nodes)["y"]->value());
    data.z = stof((*nodes)["z"]->value());

    return data;
}

XMFLOAT4 XmlHelpers::GetNodeVector4(xml_node<>* root)
{
    map<string, xml_node<>*>* nodes = GetNodeNameMap(root);
    XMFLOAT4 data = XMFLOAT4();

    data.x = stof((*nodes)["x"]->value());
    data.y = stof((*nodes)["y"]->value());
    data.z = stof((*nodes)["z"]->value());
    data.w = stof((*nodes)["w"]->value());

    return data;
}