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

void GraphicsClass::LoadGameData()
{
    xml_document<> doc;
    doc.parse<0>("config/level.xml");

    xml_node<>* root = doc.first_node();
    map<string, xml_node<>*>* siblingNodes = XmlHelpers::GetNodeList(root);

    xml_node<> *meshRoot = siblingNodes["Meshs"];
    xml_node<> *shipRoot = siblingNodes["Ships"];
    xml_node<> *modelRoot = siblingNodes["Models"];

    if (meshRoot == nullptr || shipRoot == nullptr || modelRoot == nullptr)
    {
        throw GenericException("Root xml node missing");
    }

#if defined Threaded
    auto levelMeshLoader = thread(&GraphicsClass::LoadMeshData, this, meshroot);
    auto uiMeshLoader = thread(&GraphicsClass::LoadMeshData, this, "config/ui.loader");

    levelMeshLoader.join();
    uiMeshLoader.join();

    auto levelLoader = thread(&GraphicsClass::LoadObjectData, this, modelRoot);
    auto uiLoader = thread(&GraphicsClass::LoadObjectData, this, "config/ui.loader");
    auto starLoader = thread(&GraphicsClass::GenerateStars, this, 1000);

    levelLoader.join();
    uiLoader.join();
    starLoader.join();
#elif defined Async
    auto levelMeshLoader = async(launch::async, &GraphicsClass::LoadMeshData, this, meshRoot);
    auto uiMeshLoader = async(launch::async, &GraphicsClass::LoadMeshData, this, "config/ui.loader");

    levelMeshLoader.get();
    uiMeshLoader.get();

    auto levelLoader = async(launch::async, &GraphicsClass::LoadObjectData, this, modelRoot);
    auto uiLoader = async(launch::async, &GraphicsClass::LoadObjectData, this, "config/ui.loader");
    auto starLoader = async(launch::async, &GraphicsClass::GenerateStars, this, 1000);

    levelLoader.get();
    uiLoader.get();
    starLoader.get();
#else
    LoadMeshData("config/level1.loader");
    LoadMeshData("config/ui.loader");
    LoadObjectData("config/level1.loader");
    LoadObjectData("config/ui.loader");
    GenerateStars(1000);
#endif
}

void GraphicsClass::LoadFontData(xml_node<>* meshNode)
{
            else if (sinput.compare("font") == 0)
            {
                string name, datafilename, texfilename;
                fin >> name >> datafilename >> texfilename;
#if defined Threaded
                m_allLoaders.push_back(thread(&TwoDGraphicsClass::AddFont, m_2DGraphics, name, datafilename, texfilename));
#elif defined Async
                m_allLoaders.push_back(async(launch::async, &TwoDGraphicsClass::AddFont, m_2DGraphics, name, datafilename, texfilename));
#else
                m_2DGraphics->AddFont(name, datafilename, texfilename);
#endif

            }
}

void GraphicsClass::LoadMeshData(xml_node<>* meshNode)
{
    MeshControllerClass *meshController = MeshControllerClass::getInstance();
    string sinput;
#if defined Threaded
    vector<thread> m_allLoaders = vector<thread>();
#elif defined Async
    vector<future<void>> m_allLoaders = vector<future<void>>();
#else
#endif

    vector<xml_node<>*>* allMeshNodes = XmlHelpers::GetNodeVector(meshNode);

    for (xml_node<>* &mesh : *allMeshNodes)
    {
        map<string, xml_node<>*>* meshNodes = XmlHelpers::GetNodeNameMap(mesh);
        string name = meshNodes["Name"]->value;
        string filename = meshNodes["Path"]->value;
        string type = meshNodes["Type"]->value;
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
#if defined Threaded
        m_allLoaders.push_back(thread(&MeshControllerClass::addMesh, meshController, filename, name, realType));
#elif defined Async
        m_allLoaders.push_back(async(launch::async, &MeshControllerClass::addMesh, meshController, filename, name, realType));
#else
        meshController->addMesh(filename, name, realType);
#endif

    }

           

#if defined Threaded
    // Wait for all threads to finish
    for (vector<thread>::iterator meshLoader = m_allLoaders.begin(); meshLoader != m_allLoaders.end(); ++meshLoader)
    {
        (*meshLoader).join();
    }
#elif defined Async
    // Wait for all threads to finish
    for (vector<future<void>>::iterator meshLoader = m_allLoaders.begin(); meshLoader != m_allLoaders.end(); ++meshLoader)
    {
        (*meshLoader).get();
    }
#else
#endif

}

void GraphicsClass::LoadObjectData(xml_node<>* modelsNode)
{
    string sinput;
#if defined Threaded
    vector<thread> m_allLoaders = vector<thread>();
#elif defined Async
    vector<future<void>> m_allLoaders = vector<future<void>>();
#else
#endif

    vector<xml_node<>*>* allModelNodes = XmlHelpers::GetNodeVector(modelsNode);

    for (xml_node<>* &model : *allModelNodes)
    {
        map<string, xml_node<>*>* modelNodes = XmlHelpers::GetNodeNameMap(model);
        switch (model->name)
        {
        case "Player":
            string shipName = modelNodes["ShipName"]->value;
            string team = modelNodes["Team"]->value;
            XMFLOAT3 pos = XmlHelpers::GetNodeVector3(modelNodes["SpawnPosition"]);
            XMFLOAT3 rot = XmlHelpers::GetNodeVector3(modelNodes["SpawnRotation"]);

#if defined Threaded
            m_allLoaders.push_back(thread(&ThreeDGraphicsClass::AddPlayer, m_3DGraphics, meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddPlayer, m_3DGraphics, meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos));
#else
            m_3DGraphics->AddPlayer(meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos);
#endif
            break;
        case "Ship":
            string shipName = modelNodes["ShipName"]->value;
            string team = modelNodes["Team"]->value;
            int numberOfShips = modelNodes["NumberOfShips"]->value;
            XMFLOAT3 pos = XmlHelpers::GetNodeVector3(modelNodes["SpawnPosition"]);
            XMFLOAT3 rot = XmlHelpers::GetNodeVector3(modelNodes["SpawnRotation"]);

#if defined Threaded
            m_allLoaders.push_back(thread(&ThreeDGraphicsClass::AddShip, m_3DGraphics, meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddShip, m_3DGraphics, meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos));
#else
            m_3DGraphics->AddShip(meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos);
#endif
            break;
        case "Planet":
            string meshName = modelNodes["MeshName"]->value;
            XMFLOAT3 pos = XmlHelpers::GetNodeVector3(modelNodes["SpawnPosition"]);
            XMFLOAT3 scale = XmlHelpers::GetNodeVector3(modelNodes["Scale"]);
            XMFLOAT3 rotVel = XmlHelpers::GetNodeVector3(modelNodes["RotationVelocity"]);

#if defined Threaded
            m_allLoaders.push_back(thread(&ThreeDGraphicsClass::AddPlanet, m_3DGraphics, meshName, pos, scale, rotVel));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddPlanet, m_3DGraphics, meshName, pos, scale, rotVel));
#else
            m_3DGraphics->AddPlanet(meshName, pos, scale, rotVel);
#endif  
            break;
        case "Star":
            string meshName = modelNodes["MeshName"]->value;
            XMFLOAT3 pos = XmlHelpers::GetNodeVector3(modelNodes["SpawnPosition"]);
            XMFLOAT3 scale = XmlHelpers::GetNodeVector3(modelNodes["Scale"]);
            XMFLOAT3 rotVel = XmlHelpers::GetNodeVector3(modelNodes["RotationVelocity"]);

#if defined Threaded
            m_allLoaders.push_back(thread(&ThreeDGraphicsClass::AddStar, m_3DGraphics, meshName, pos, scale, rotVel));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddStar, m_3DGraphics, meshName, pos, scale, rotVel));
#else
            m_3DGraphics->AddStar(meshName, pos, scale, rotVel);
#endif
            break;
        case "Text":
            string name = modelNodes["Name"]->value;
            string initText = modelNodes["InitText"]->value;
            string fontName = modelNodes["FontName"]->value;
            XMFLOAT2 pos = XmlHelpers::GetNodeVector2(modelNodes["SpawnPosition"]);
            XMFLOAT2 scale = XmlHelpers::GetNodeVector2(modelNodes["Scale"]);
            XMFLOAT4 color = XmlHelpers::GetNodeVector4(modelNodes["RotationVelocity"]);

#if defined Threaded
            m_allLoaders.push_back(thread(&TwoDGraphicsClass::AddText, m_2DGraphics, name, initText, fontname, pos, scale, color));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &TwoDGraphicsClass::AddText, m_2DGraphics, name, initText, fontname, pos, scale, color));
#else
            m_2DGraphics->AddText(name, initText, fontname, pos, scale, color);
#endif 
            break;
        case "Bitmap":
            string meshName = modelNodes["MeshName"]->value;
            XMFLOAT2 pos = XmlHelpers::GetNodeVector2(modelNodes["SpawnPosition"]);
            XMFLOAT2 scale = XmlHelpers::GetNodeVector2(modelNodes["Scale"]);

#if defined Threaded
            m_allLoaders.push_back(thread(&TwoDGraphicsClass::AddBitmap, m_2DGraphics, meshName, pos, scale));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &TwoDGraphicsClass::AddBitmap, m_2DGraphics, meshName, pos, scale));
#else
            m_2DGraphics->AddBitmap(meshName, pos, scale);
#endif 
            break;
        }
        
    }

#if defined Threaded
    // Wait for all threads to finish
    for (vector<thread>::iterator loader = m_allLoaders.begin(); loader != m_allLoaders.end(); ++loader)
    {
        (*loader).join();
    }
#elif defined Async
    // Wait for all threads to finish
    for (vector<future<void>>::iterator loader = m_allLoaders.begin(); loader != m_allLoaders.end(); ++loader)
    {
        (*loader).get();
    }
#else
#endif
}

void GraphicsClass::GenerateStars(int starCount)
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