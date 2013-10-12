#include "GarphicsClass.h"
#include "WindowClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderControllerClass.h"
#include "MeshClass.h"
#include "ThreeDGraphicsClassh.h"
#include "TwoDGraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_Window = 0;
	m_D3D = 0;
	m_Camera = 0;
	m_Shader = 0;
	m_2DGraphics = 0;
	m_3DGraphics = 0;
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

	//Window Init
	m_Window = new WindowClass();
	if(!m_Window)
		return false;

	result = m_Window->Initialize(hInstance, iCmdShow);
	if(!result)
		return false;

	//Directx Device Init
	m_D3D = new D3DClass();
	if(!m_D3D)
		return false;

	result = m_D3D->Initialize(m_Window->getWindowWidth(), m_Window->getWindowHeight(), m_Window->VSYNC_ENABLED, m_Window->gethWnd(), m_Window->FULL_SCREEN, m_Window->SCREEN_DEPTH, m_Window->SCREEN_NEAR);
	if(!result)
		return false;

	//Create the camera object
	m_Camera = new CameraClass;
	if(!m_Camera)
		return false;

	result = m_Camera->Initialize();
	if(!result)
		return false;

	//Shader Init
	m_Shader = new ShaderControllerClass();
	if(!m_Shader)
		return false;

	result = m_Shader->Initialize(m_D3D->GetDevice(), m_Window->gethWnd());
	if(!result)
		return false;

	//Init all 2D objects, UI, menus,...
	m_2DGraphics = new TwoDGraphicsClass();
	if(!m_2DGraphics)
		return false;

	result = m_2DGraphics->Initialize(m_D3D->GetDevice(), m_Window->getScreenWidth(), m_Window->getScreenHeight());
	if(!result)
		return false;

	//Init all 3d objects
	m_3DGraphics = new ThreeDGraphicsClass();
	if(!m_3DGraphics)
		return false;

	result = m_3DGraphics->Initialize(m_D3D->GetDevice());
	if(!result)
		return false;

	return true;
}

WindowClass* GraphicsClass::getWindowObj()
{
	return m_Window;
}

bool GraphicsClass::Render()
{
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Generate the view matrix based on the camera's position
	m_Camera->Render();

	//Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	result = m_3DGraphics->RenderAll(m_D3D, m_Shader, m_Camera);
	if (!result)
		return false;

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

CameraClass* GraphicsClass::getCamera()
{
	return m_Camera;
}