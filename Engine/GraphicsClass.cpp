#include "GraphicsClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderControllerClass.h"
#include "MeshClass.h"
#include "ThreeDGraphicsClass.h"
#include "TwoDGraphicsClass.h"
#include "Timer.h"
#include "MeshControllerClass.h"
#include "WindowsHelpers.h"
#include "GenericException.h"
#include <fstream>
#include <string>
#include "rapidxml\rapidxml.hpp"
#include "XmlHelpers.h"

using namespace std;
using namespace DirectX;
using namespace rapidxml;

GraphicsClass::GraphicsClass()
{
    m_Shader = nullptr;
    m_2DGraphics = nullptr;
    m_3DGraphics = nullptr;
    m_fps = 0;
    m_fpsCount = 0;
}

GraphicsClass::~GraphicsClass()
{
}

void GraphicsClass::Initialize(int iCmdShow)
{
    m_fpsTimer = Timer();

    D3DClass::getInstance()->Initialize();
    CameraClass::getInstance()->Initialize();
    MeshControllerClass::getInstance()->Initialize();

    //Shader Init
    m_Shader = new ShaderControllerClass();
    m_Shader->Initialize();

    //Init all 2D objects, UI, menus,...
    m_2DGraphics = new TwoDGraphicsClass();
    m_2DGraphics->Initialize();

    //Init all 3d objects
    m_3DGraphics = new ThreeDGraphicsClass();
    m_3DGraphics->Initialize();
}

void GraphicsClass::Shutdown()
{
    if (m_2DGraphics)
    {
        m_2DGraphics->Shutdown();
        delete m_2DGraphics;
        m_2DGraphics = 0;
    }

    if (m_3DGraphics)
    {
        delete m_3DGraphics;
        m_3DGraphics = 0;
    }

    if (m_Shader)
    {
        m_Shader->Shutdown();
        delete m_Shader;
        m_Shader = 0;
    }

    MeshControllerClass::getInstance()->Shutdown();
    delete MeshControllerClass::getInstance();

    CameraClass::getInstance()->Shutdown();
    delete CameraClass::getInstance();

    D3DClass::getInstance()->Shutdown();
    delete D3DClass::getInstance();
}

void GraphicsClass::PreProcessing()
{
    m_3DGraphics->PreProcessing();
}

void GraphicsClass::Render()
{
    if (!m_fpsTimer.is_started())
    {
        m_fpsTimer.start();
    }

    if (m_fpsTimer.get_ticks() > 1000.0f)
    {
        m_fps = m_fpsCount;
        m_fpsCount = 0;
        m_fpsTimer.restart();
    }

    // Clear the buffers to begin the scene.
    D3DClass::getInstance()->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    //Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
    m_3DGraphics->RenderAll(m_Shader);

    m_2DGraphics->RenderAll(m_Shader, m_fps);

    // Present the rendered scene to the screen.
    D3DClass::getInstance()->EndScene();

    m_fpsCount++;
}

void GraphicsClass::LoadGameData(string filename)
{
    char* cfilename = new char[filename.size() + 1];
    strcpy_s(cfilename, sizeof(char)*(filename.size() + 1), filename.c_str());

    xml_document<> doc;
    doc.parse<0>(cfilename);

    xml_node<>* root = doc.first_node();
    map<string, xml_node<>*>* siblingNodes = XmlHelpers::GetNodeNameMap(root);

    xml_node<> *meshRoot = (*siblingNodes)["Meshs"];
    xml_node<> *shipRoot = (*siblingNodes)["Ships"];
    xml_node<> *modelRoot = (*siblingNodes)["Models"];
    xml_node<> *fontRoot = (*siblingNodes)["Fonts"];

    if (meshRoot == nullptr || shipRoot == nullptr || modelRoot == nullptr)
    {
        throw GenericException("Root xml node missing");
    }

    auto meshLoader = async(launch::async, &GraphicsClass::LoadMeshData, this, meshRoot);
    auto fontLoader = async(launch::async, &GraphicsClass::LoadFontData, this, fontRoot);
    meshLoader.get();
    fontLoader.get();

    auto shipLoader = async(launch::async, &GraphicsClass::LoadShipData, this, shipRoot);
    map<string, ShipData>* shipData = shipLoader.get();

    auto objectLoader = async(launch::async, &GraphicsClass::LoadObjectData, this, modelRoot, shipData);
    objectLoader.get();
}

void GraphicsClass::LoadData()
{
    vector<string> files = { "config/Level1.xml", "config/UI.xml" };

    for (string &file : files)
    {
        async(launch::async, &GraphicsClass::LoadGameData, this, file);
    }
}

void GraphicsClass::LoadFontData(xml_node<>* fontNode)
{
    if (fontNode == nullptr)
    {
        return;
    }

    vector<future<void>> m_allLoaders = vector<future<void>>();
    vector<xml_node<>*>* allFontNodes = XmlHelpers::GetNodeVector(fontNode);

    for (xml_node<>* &font : *allFontNodes)
    {
        map<string, xml_node<>*>* fontNodes = XmlHelpers::GetNodeNameMap(font);
        string name = (*fontNodes)["Name"]->value();
        string datafilename = (*fontNodes)["DataFilePath"]->value();
        string texfilename = (*fontNodes)["TextureFilePath"]->value();
        m_allLoaders.push_back(async(launch::async, &TwoDGraphicsClass::AddFont, m_2DGraphics, name, datafilename, texfilename));
    }

    // Wait for all threads to finish
    for (vector<future<void>>::iterator meshLoader = m_allLoaders.begin(); meshLoader != m_allLoaders.end(); ++meshLoader)
    {
        (*meshLoader).get();
    }
}

void GraphicsClass::LoadMeshData(xml_node<>* meshNode)
{
    if (meshNode == nullptr)
    {
        return;
    }

    MeshControllerClass *meshController = MeshControllerClass::getInstance();
    vector<future<void>> m_allLoaders = vector<future<void>>();

    vector<xml_node<>*>* allMeshNodes = XmlHelpers::GetNodeVector(meshNode);

    for (xml_node<>* &mesh : *allMeshNodes)
    {
        map<string, xml_node<>*>* meshNodes = XmlHelpers::GetNodeNameMap(mesh);
        string name = (*meshNodes)["Name"]->value();
        string filename = (*meshNodes)["Path"]->value();
        string type = (*meshNodes)["Type"]->value();
        MeshClass::MeshType realType;
        
        if (type.compare("THREED") == 0)
        {
            realType = MeshClass::THREED;
        }
        else if (type.compare("TWOD") == 0)
        {
            realType = MeshClass::TWOD;
        }
        else
        {
            throw GenericException("Unknown mesh type.");
        }
        m_allLoaders.push_back(async(launch::async, &MeshControllerClass::addMesh, meshController, filename, name, realType));
    }

    // Wait for all threads to finish
    for (vector<future<void>>::iterator meshLoader = m_allLoaders.begin(); meshLoader != m_allLoaders.end(); ++meshLoader)
    {
        (*meshLoader).get();
    }
}

map<string, ShipData>* GraphicsClass::LoadShipData(xml_node<>* modelsNode)
{
    string sinput;
    vector<xml_node<>*>* allModelNodes = XmlHelpers::GetNodeVector(modelsNode);
    map<string, ShipData>* allShipData = new map<string, ShipData>();

    for (xml_node<>* &model : *allModelNodes)
    {
        map<string, xml_node<>*>* modelNodes = XmlHelpers::GetNodeNameMap(model);
        
        ShipData data = ShipData();
        data.ShipName = (*modelNodes)["ShipName"]->value();
        data.MeshName = (*modelNodes)["MeshName"]->value();
        data.Scale = XmlHelpers::GetNodeVector3((*modelNodes)["Scale"]);
        data.TotalHealth = atoi((*modelNodes)["TotalHealth"]->value());
        data.TotalShields = atoi((*modelNodes)["TotalShields"]->value());
        data.TotalEnergy = atoi((*modelNodes)["TotalEnergy"]->value());
        data.EnergyCost = atoi((*modelNodes)["EnergyCost"]->value());
        data.NumberOfTorpedos = atoi((*modelNodes)["NumberOfTorpedos"]->value());

        (*allShipData)[data.ShipName] = data;
    }

    return allShipData;
}

void GraphicsClass::LoadObjectData(xml_node<>* modelsNode, map<string, ShipData>* shipData)
{
    string sinput;
    vector<future<void>> m_allLoaders = vector<future<void>>();

    vector<xml_node<>*>* allModelNodes = XmlHelpers::GetNodeVector(modelsNode);

    for (xml_node<>* &model : *allModelNodes)
    {
        map<string, xml_node<>*>* modelNodes = XmlHelpers::GetNodeNameMap(model);
        if (model->name() == "Player")
        {
            string shipName = (*modelNodes)["ShipName"]->value();
            ShipData data = (*shipData)[shipName];
            string team = (*modelNodes)["Team"]->value();
            XMFLOAT3 pos = XmlHelpers::GetNodeVector3((*modelNodes)["SpawnPosition"]);
            XMFLOAT3 rot = XmlHelpers::GetNodeVector3((*modelNodes)["SpawnRotation"]);

            m_allLoaders.push_back(
                async(
                launch::async, 
                &ThreeDGraphicsClass::AddPlayer, 
                m_3DGraphics, 
                data.MeshName, 
                pos, 
                data.Scale, 
                data.TotalHealth, 
                data.TotalShields, 
                data.TotalEnergy, 
                data.EnergyCost, 
                data.NumberOfTorpedos));
        }
        else if (model->name() == "Ship")
        {
            string shipName = (*modelNodes)["ShipName"]->value();
            ShipData data = (*shipData)[shipName];
            string team = (*modelNodes)["Team"]->value();
            int numberOfShips = atoi((*modelNodes)["NumberOfShips"]->value());
            XMFLOAT3 pos = XmlHelpers::GetNodeVector3((*modelNodes)["SpawnPosition"]);
            XMFLOAT3 rot = XmlHelpers::GetNodeVector3((*modelNodes)["SpawnRotation"]);

            m_allLoaders.push_back(
                async(
                launch::async, 
                &ThreeDGraphicsClass::AddShip, 
                m_3DGraphics,
                data.MeshName,
                pos,
                data.Scale,
                data.TotalHealth,
                data.TotalShields,
                data.TotalEnergy,
                data.EnergyCost,
                data.NumberOfTorpedos));
        }
        else if (model->name() == "Planet")
        {
            string meshName = (*modelNodes)["MeshName"]->value();
            XMFLOAT3 pos = XmlHelpers::GetNodeVector3((*modelNodes)["SpawnPosition"]);
            XMFLOAT3 scale = XmlHelpers::GetNodeVector3((*modelNodes)["Scale"]);
            XMFLOAT3 rotVel = XmlHelpers::GetNodeVector3((*modelNodes)["RotationVelocity"]);

            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddPlanet, m_3DGraphics, meshName, pos, scale, rotVel));
        }
        else if (model->name() == "Star")
        {
            string meshName = (*modelNodes)["MeshName"]->value();
            XMFLOAT3 pos = XmlHelpers::GetNodeVector3((*modelNodes)["SpawnPosition"]);
            XMFLOAT3 scale = XmlHelpers::GetNodeVector3((*modelNodes)["Scale"]);
            XMFLOAT3 rotVel = XmlHelpers::GetNodeVector3((*modelNodes)["RotationVelocity"]);

            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddStar, m_3DGraphics, meshName, pos, scale, rotVel));
        }
        else if (model->name() == "Text")
        {
            string name = (*modelNodes)["Name"]->value();
            string initText = (*modelNodes)["StartingText"]->value();
            string fontName = (*modelNodes)["FontName"]->value();
            XMFLOAT2 pos = XmlHelpers::GetNodeVector2((*modelNodes)["Position"]);
            XMFLOAT2 scale = XmlHelpers::GetNodeVector2((*modelNodes)["Scale"]);
            XMFLOAT4 color = XmlHelpers::GetNodeVector4((*modelNodes)["Color"]);

            m_allLoaders.push_back(async(launch::async, &TwoDGraphicsClass::AddText, m_2DGraphics, name, initText, fontName, pos, scale, color));
        }
        else if (model->name() == "Bitmap")
        {
            string meshName = (*modelNodes)["MeshName"]->value();
            XMFLOAT2 pos = XmlHelpers::GetNodeVector2((*modelNodes)["Position"]);
            XMFLOAT2 scale = XmlHelpers::GetNodeVector2((*modelNodes)["Scale"]);

            m_allLoaders.push_back(async(launch::async, &TwoDGraphicsClass::AddBitmap, m_2DGraphics, meshName, pos, scale));
        }
        else if (model->name() == "BackgroundStars")
        {
            int numberOfStars = atoi((*modelNodes)["Count"]->value());
            LoadBackgroundStars(numberOfStars);
        }        
    }

    // Wait for all threads to finish
    for (vector<future<void>>::iterator loader = m_allLoaders.begin(); loader != m_allLoaders.end(); ++loader)
    {
        (*loader).get();
    }
}

void GraphicsClass::LoadBackgroundStars(int starCount)
{
    float MAX_DISTANCE = 200.0f;

    for (int i = 0; i < starCount; i++)
    {
        float posX = WindowsHelpers::RandomFloat(0.0f, MAX_DISTANCE);
        float posY = WindowsHelpers::RandomFloat(0.0f, MAX_DISTANCE);
        float posZ = WindowsHelpers::RandomFloat(0.0f, MAX_DISTANCE);

        if (posX < MAX_DISTANCE / 2.0f && posY < MAX_DISTANCE / 2.0f && posZ < MAX_DISTANCE / 2.0f)
        {
            int randomSeed = rand() % 3;
            if (randomSeed == 0)
            {
                posX += MAX_DISTANCE / 2.0f;
            }
            else if (randomSeed == 1)
            {
                posY += MAX_DISTANCE / 2.0f;
            }
            else if (randomSeed == 2)
            {
                posZ += MAX_DISTANCE / 2.0f;
            }
        }

        if (int(posX) % 2 == 1)
            posX *= -1.0f;
        if (int(posY) % 2 == 1)
            posY *= -1.0f;
        if (int(posZ) % 2 == 1)
            posZ *= -1.0f;

        m_3DGraphics->AddStar("starMesh", XMFLOAT3(posX, posY, posZ), XMFLOAT3(0.15f, 0.15f, 0.15f), XMFLOAT3(0.0f, 0.0f, 0.0f));
    }
}

void GraphicsClass::HandleEvents(EventClass* events)
{
    m_3DGraphics->HandleEvents(events);
}