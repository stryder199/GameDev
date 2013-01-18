#include "TwoDGraphicsClass.h"
#include "BitmapClass.h"
#include "D3DClass.h"
#include "ShaderClass.h"
#include "CameraClass.h"

TwoDGraphicsClass::TwoDGraphicsClass()
{
	m_Bitmap = 0;
}

TwoDGraphicsClass::TwoDGraphicsClass(const TwoDGraphicsClass& other)
{
}

TwoDGraphicsClass::~TwoDGraphicsClass()
{
}

bool TwoDGraphicsClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight)
{
	bool result;

	m_Bitmap = new BitmapClass();
	if(!m_Bitmap)
		return false;

	m_Bitmap->Initialize(device, screenWidth, screenHeight, L"../Engine/data/seafloor.dds", 256, 256);
	if(!result)
	{
		return false;
	}

	return true;
}

bool TwoDGraphicsClass::RenderAll(D3DClass* D3D, CameraClass* camera, ShaderClass* shader)
{
	bool result;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	viewMatrix = *camera->GetViewMatrix();
	D3D->GetWorldMatrix(worldMatrix);
	D3D->GetProjectionMatrix(projectionMatrix);
	D3D->GetOrthoMatrix(orthoMatrix);



	//Turn off the ZBuffer since 2D elements have no Z component
	D3D->TurnZBufferOff();

	//Render all the 2D objects
	result = m_Bitmap->Render(D3D->GetDeviceContext(), 100, 100);
	if(!result)
		return false;

	//Render the model using the shader
	result = shader->Render(D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, *camera->GetViewMatrix(), orthoMatrix,
		m_Bitmap->GetTexture(),lightSource->GetDirection(), lightSource->GetAmbientColor(), lightSource->GetDiffuseColor());
	if(!result)
		return false;

	//Turn the ZBuffer back on for future 3d rendering
	D3D->TurnZBufferOn();

	return true;
}