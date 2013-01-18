#include "GarphicsClass.h"
#include "WindowClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderClass.h"
#include "MeshClass.h"
#include "ThreeDGraphicsClassh.h"
#include "TwoDGraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_Window = 0;
	m_D3D = 0;
	m_Camera = 0;
	m_Shader = 0;
	m_Model = 0;
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
	D3D11_INPUT_ELEMENT_DESC* polygonLayout;
	int layoutCount;

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
	m_Shader = new ShaderClass();
	if(!m_Shader)
		return false;

	//Now setup the layout of the data that goes into the shader
	//This setup needs to match the VertexType structure in the MeshClass and in the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset= 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	result = m_Shader->Initialize(m_D3D->GetDevice(), m_Window->gethWnd());
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
	m_3DGraphics->RenderAll(m_D3D, m_Shader, m_Camera);

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

CameraClass* GraphicsClass::getCamera()
{
	return m_Camera;
}