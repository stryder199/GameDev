#include "GarphicsClass.h"
#include "WindowClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderControllerClass.h"
#include "MeshClass.h"
#include "ThreeDGraphicsClassh.h"
#include "TwoDGraphicsClass.h"
#include "ActorsClass.h"
#include "Timer.h"

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