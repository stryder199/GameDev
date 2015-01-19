#include "GraphicsClass.h"
#include "WindowClass.h"
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

GraphicsClass::GraphicsClass()
{
    m_Shader = 0;
    m_2DGraphics = 0;
    m_3DGraphics = 0;
    m_fps = 0;
    m_fpsCount = 0;
}

GraphicsClass::~GraphicsClass()
{
}

void GraphicsClass::Initialize(HINSTANCE hInstance, int iCmdShow)
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
        m_3DGraphics->Shutdown();
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
#if defined Threaded
    auto levelMeshLoader = thread(&GraphicsClass::LoadMeshData, this, "config/level1.loader");
    auto uiMeshLoader = thread(&GraphicsClass::LoadMeshData, this, "config/ui.loader");

    levelMeshLoader.join();
    uiMeshLoader.join();

    auto levelLoader = thread(&GraphicsClass::LoadObjectData, this, "config/level1.loader");
    auto uiLoader = thread(&GraphicsClass::LoadObjectData, this, "config/ui.loader");
    auto starLoader = thread(&GraphicsClass::GenerateStars, this, 1000);

    levelLoader.join();
    uiLoader.join();
    starLoader.join();
#elif defined Async
    auto levelMeshLoader = async(launch::async, &GraphicsClass::LoadMeshData, this, "config/level1.loader");
    auto uiMeshLoader = async(launch::async, &GraphicsClass::LoadMeshData, this, "config/ui.loader");

    levelMeshLoader.get();
    uiMeshLoader.get();

    auto levelLoader = async(launch::async, &GraphicsClass::LoadObjectData, this, "config/level1.loader");
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

void GraphicsClass::LoadMeshData(string filename)
{
    MeshControllerClass *meshController = MeshControllerClass::getInstance();
    string sinput;
#if defined Threaded
    vector<thread> m_allLoaders = vector<thread>();
#elif defined Async
    vector<future<void>> m_allLoaders = vector<future<void>>();
#else
#endif

    ifstream fin(filename);
    if (fin.fail())
    {
        throw GenericException("File not found");
    }

    fin >> sinput;

    while (!fin.eof())
    {
        if (sinput.compare("mesh") == 0)
        {
            string name, filename, type;
            MeshClass::MeshType realType;

            fin >> name >> filename >> type;
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
        else
        {
            getline(fin, sinput);
        }

        fin >> sinput;
    }
    fin.close();

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

void GraphicsClass::LoadObjectData(string filename)
{
    string sinput;
#if defined Threaded
    vector<thread> m_allLoaders = vector<thread>();
#elif defined Async
    vector<future<void>> m_allLoaders = vector<future<void>>();
#else
#endif

    ifstream fin(filename);
    if (fin.fail())
    {
        throw GenericException("File not found");
    }
    fin >> sinput;

    while (!fin.eof())
    {
        if (sinput.compare("player") == 0)
        {
            string meshName;
            XMFLOAT3 pos, scale;
            int totalHealth, totalShields, totalEnergy, energyCost, torpedos;
            
            fin >> meshName >> pos.x >> pos.y >> pos.z >> scale.x >> scale.y >> scale.z
                >> totalHealth >> totalShields >> totalEnergy >> energyCost >> torpedos;

#if defined Threaded
            m_allLoaders.push_back(thread(&ThreeDGraphicsClass::AddPlayer, m_3DGraphics, meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddPlayer, m_3DGraphics, meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos));
#else
            m_3DGraphics->AddPlayer(meshName, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos);
#endif
        }
        else if (sinput.compare("planet") == 0)
        {
            string meshName;
            XMFLOAT3 pos, scale, rotVel;

            fin >> meshName >> pos.x >> pos.y >> pos.z >> scale.x >> scale.y >> scale.z
                >> rotVel.x >> rotVel.y >> rotVel.z;

#if defined Threaded
            m_allLoaders.push_back(thread(&ThreeDGraphicsClass::AddPlanet, m_3DGraphics, meshName, pos, scale, rotVel));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddPlanet, m_3DGraphics, meshName, pos, scale, rotVel));
#else
            m_3DGraphics->AddPlanet(meshName, pos, scale, rotVel);
#endif  
        }
        else if (sinput.compare("star") == 0)
        {
            string meshName;
            XMFLOAT3 pos, scale, rotVel;

            fin >> meshName >> pos.x >> pos.y >> pos.z >> scale.x >> scale.y >> scale.z
                >> rotVel.x >> rotVel.y >> rotVel.z;

#if defined Threaded
            m_allLoaders.push_back(thread(&ThreeDGraphicsClass::AddStar, m_3DGraphics, meshName, pos, scale, rotVel));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &ThreeDGraphicsClass::AddStar, m_3DGraphics, meshName, pos, scale, rotVel));
#else
            m_3DGraphics->AddStar(meshName, pos, scale, rotVel);
#endif
        }
        else if (sinput.compare("text") == 0)
        {
            string name, initText, fontname;
            XMFLOAT2 pos, scale;
            XMFLOAT4 color;

            fin >> name >> initText >> fontname >> pos.x >> pos.y >> scale.x >> scale.y
                >> color.x >> color.y >> color.z >> color.w;

#if defined Threaded
            m_allLoaders.push_back(thread(&TwoDGraphicsClass::AddText, m_2DGraphics, name, initText, fontname, pos, scale, color));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &TwoDGraphicsClass::AddText, m_2DGraphics, name, initText, fontname, pos, scale, color));
#else
            m_2DGraphics->AddText(name, initText, fontname, pos, scale, color);
#endif 
        }
        else if (sinput.compare("bitmap") == 0)
        {
            string meshName;
            XMFLOAT2 pos, scale;

            fin >> meshName >> pos.x >> pos.y >> scale.x >> scale.y;

#if defined Threaded
            m_allLoaders.push_back(thread(&TwoDGraphicsClass::AddBitmap, m_2DGraphics, meshName, pos, scale));
#elif defined Async
            m_allLoaders.push_back(async(launch::async, &TwoDGraphicsClass::AddBitmap, m_2DGraphics, meshName, pos, scale));
#else
            m_2DGraphics->AddBitmap(meshName, pos, scale);
#endif 
        }
        else
        {
            getline(fin, sinput);
        }

        fin >> sinput;
    }
    fin.close();

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