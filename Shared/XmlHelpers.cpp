#include "XmlHelpers.h"
#include "rapidxml\rapidxml.hpp"

using namespace std;
using namespace rapidxml;

vector<xml_node<>*>* GetNodeVector(xml_node<>* root)
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

map<string, xml_node<>*>* GetNodeNameMap(xml_node<>* root)
{
    map<string, xml_node<>*>* siblingNodes = new map<string, xml_node<>*>();

    xml_node<>* node = root->first_node();
    if (node != nullptr)
    {
        siblingNodes[node->name] = node;
    }

    while (node->next_sibling() != nullptr)
    {
        node = node->next_sibling();
        siblingNodes[node->name] = node;
    }

    return siblingNodes;
}