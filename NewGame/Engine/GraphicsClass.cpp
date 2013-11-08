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

GraphicsClass::GraphicsClass()
{
	m_Shader = 0;
	m_2DGraphics = 0;
	m_3DGraphics = 0;
	m_fps = 0;
	m_fpsCount = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(HINSTANCE hInstance, int iCmdShow)
{
	bool result;

	m_fpsTimer = Timer();

	result = D3DClass::getInstance()->Initialize();
	if(!result)
		return false;

	result = CameraClass::getInstance()->Initialize();
	if(!result)
		return false;

	//Shader Init
	m_Shader = new ShaderControllerClass();
	if(!m_Shader)
		return false;

	result = m_Shader->Initialize();
	if(!result)
		return false;

	//Init all 2D objects, UI, menus,...
	m_2DGraphics = new TwoDGraphicsClass();
	if(!m_2DGraphics)
		return false;

	result = m_2DGraphics->Initialize();
	if(!result)
		return false;

	//Init all 3d objects
	m_3DGraphics = new ThreeDGraphicsClass();
	if(!m_3DGraphics)
		return false;

	result = m_3DGraphics->Initialize();
	if(!result)
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	bool result;

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
	result = m_3DGraphics->RenderAll(m_Shader);
	if (!result)
		return false;

	result = m_2DGraphics->RenderAll(m_Shader, m_fps);
	if (!result)
		return false;

	// Present the rendered scene to the screen.
	D3DClass::getInstance()->EndScene();

	m_fpsCount++;

	return true;
}

bool GraphicsClass::LoadGameData()
{
	bool result;

	auto levelMeshLoader = async(&GraphicsClass::LoadMeshData, this, "config/level1.loader");
	auto uiMeshLoader = async(&GraphicsClass::LoadMeshData, this, "config/ui.loader");
	auto threePrimMeshLoader = async(&GraphicsClass::LoadMeshData, this, "config/3dprim.loader");
	auto twoPrimMeshLoader = async(&GraphicsClass::LoadMeshData, this, "config/2dprim.loader");

	result = levelMeshLoader.get();
	if (!result)
		return false;
	result = uiMeshLoader.get();
	if (!result)
		return false;
	result = threePrimMeshLoader.get();
	if (!result)
		return false;
	result = twoPrimMeshLoader.get();
	if (!result)
		return false;

	auto levelLoader = async(&GraphicsClass::LoadObjectData, this, "config/level1.loader");
	auto uiLoader = async(&GraphicsClass::LoadObjectData, this, "config/ui.loader");

	result = levelLoader.get();
	if (!result)
		return false;
	result = uiLoader.get();
	if (!result)
		return false;

	return true;
}

bool GraphicsClass::LoadMeshData(string filename)
{
	MeshControllerClass *meshController = MeshControllerClass::getInstance();
	string sinput;
	vector<future<bool>> m_allLoaders = vector<future<bool>>();
	bool result;

	ifstream fin(filename);
	if (!fin) 
	{
		cerr << "Could not find file: " << filename << endl;
		return false;
	}

	fin >> sinput;

	while (sinput != "")
	{
		if (sinput.compare("mesh") == 0)
		{
			string name, filename, type;
			MeshClass::MeshType realType;

			fin >> name >> filename >> type;
			if (type.compare("THREED"))
			{
				realType = MeshClass::THREED;
			}
			else if (type.compare("TWOD"))
			{
				realType = MeshClass::TWOD;
			}
			else
			{
				return false;
			}

			auto meshResult = async(&MeshControllerClass::addObjMesh, meshController, filename, name, realType);
			m_allLoaders.push_back(meshResult);
		}
		else if (sinput.compare("font") == 0)
		{
			string name, datafilename, texfilename;
			fin >> name >> datafilename >> texfilename;

			auto fontResult = async(&TwoDGraphicsClass::AddText, meshController, name, datafilename, texfilename);
			m_allLoaders.push_back(fontResult);
		}
		else
		{
			getline(fin, sinput);
		}

		fin >> sinput;
	}
	fin.close();

	// Wait for all asyncs to finish
	vector<future<bool>>::iterator meshLoader;
	for (meshLoader = m_allLoaders.begin(); meshLoader != m_allLoaders.end(); ++meshLoader)
	{
		result = (*meshLoader).get();
		if (!result)
			return false;
	}

	return true;
}

bool GraphicsClass::LoadObjectData(string filename)
{
	string sinput;
	ifstream fin(filename);
	vector<future<bool>> m_allLoaders = vector<future<bool>>();
	bool result;
	if (!fin)
	{
		cerr << "Could not find file: " << filename << endl;
		return false;
	}

	fin >> sinput;

	while (sinput != "")
	{
		if (sinput.compare("player") == 0)
		{
			string meshName;
			XMFLOAT3 pos, scale;
			int totalHealth, totalShields, totalEnergy, energyCost, torpedos;
			
			fin >> meshName >> pos.x >> pos.y >> pos.z >> scale.x >> scale.y >> scale.z
				>> totalHealth >> totalShields >> totalEnergy >> energyCost >> torpedos;

			auto playerResult = async(&ThreeDGraphicsClass::AddPlayer, m_3DGraphics, meshName, pos, scale, totalHealth, totalShields,
										totalEnergy, energyCost, torpedos);
			m_allLoaders.push_back(playerResult);
		}
		else if (sinput.compare("planet") == 0)
		{
			string meshName;
			XMFLOAT3 pos, scale, rotVel;

			fin >> meshName >> pos.x >> pos.y >> pos.z >> scale.x >> scale.y >> scale.z
				>> rotVel.x >> rotVel.y >> rotVel.z;

			auto planetResult = async(&ThreeDGraphicsClass::AddPlanet, m_3DGraphics, meshName, pos, scale, rotVel);
			m_allLoaders.push_back(planetResult);
		}
		else if (sinput.compare("star") == 0)
		{
			string meshName;
			XMFLOAT3 pos, scale, rotVel;

			fin >> meshName >> pos.x >> pos.y >> pos.z >> scale.x >> scale.y >> scale.z
				>> rotVel.x >> rotVel.y >> rotVel.z;

			auto starResult = async(&ThreeDGraphicsClass::AddStar, m_3DGraphics, meshName, pos, scale, rotVel);
			m_allLoaders.push_back(starResult);
		}
		else if (sinput.compare("text") == 0)
		{
			string name, initText, fontname;
			XMFLOAT2 pos, scale;
			XMFLOAT4 color;

			fin >> name >> initText >> fontname >> pos.x >> pos.y >> scale.x >> scale.y
				>> color.x >> color.y >> color.z >> color.w;

			auto textResult = async(&TwoDGraphicsClass::AddText, m_2DGraphics, name, initText, fontname, pos, scale, color);
			m_allLoaders.push_back(textResult);
		}
		else if (sinput.compare("bitmap") == 0)
		{
			string meshName;
			XMFLOAT2 pos, scale;

			fin >> meshName >> pos.x >> pos.y >> scale.x >> scale.y;

			auto bitmapResult = async(&TwoDGraphicsClass::AddBitmap, m_2DGraphics, meshName, pos, scale);
			m_allLoaders.push_back(bitmapResult);
		}
		else
		{
			getline(fin, sinput);
		}

		fin >> sinput;
	}
	fin.close();

	// Wait for all asyncs to finish
	vector<future<bool>>::iterator loader;
	for (loader = m_allLoaders.begin(); loader != m_allLoaders.end(); ++loader)
	{
		result = (*loader).get();
		if (!result)
			return false;
	}

	return true;
}

bool GraphicsClass::GenerateStars(int starCount)
{
	bool result;
	float MAX_DISTANCE = 200.0f;

	for (int i = 0; i < starCount; i++)
	{
		float posX = RandomFloat(0.0f, MAX_DISTANCE);
		float posY = RandomFloat(0.0f, MAX_DISTANCE);
		float posZ = RandomFloat(0.0f, MAX_DISTANCE);

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

		result = m_3DGraphics->AddStar("starMesh", XMFLOAT3(posX, posY, posZ), XMFLOAT3(100.00f, 100.00f, 100.00f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		if (!result)
			return false;
	}

	return true;
}