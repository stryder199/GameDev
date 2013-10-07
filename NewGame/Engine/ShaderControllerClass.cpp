#include "ShaderControllerClass.h"
#include "PixelShaderClass.h"
#include "VertexShaderClass.h"

ShaderControllerClass::ShaderControllerClass()
{
}

ShaderControllerClass::~ShaderControllerClass()
{
}

bool ShaderControllerClass::Initialize(ID3D11Device* device, HWND hwnd, D3D11_INPUT_ELEMENT_DESC* threeDPolygonLayout, int threeDLayoutCount, 
										D3D11_INPUT_ELEMENT_DESC* twoDPolygonLayout, int twoDLayoutCount)
{
	bool result;
	WCHAR* ThreeDPSFilename = L"shaders\\threeDPixelShader.hlsl";
	WCHAR* TwoDPSFilename = L"shaders\\twoDPixelShader.hlsl";
	WCHAR* ThreeDVSFilename = L"shaders\\threeDVertexShader.hlsl";
	WCHAR* TwoDVSFilename = L"shaders\\twoDVertexShader.hlsl";

	m_threeDPixelShader = new PixelShaderClass();
	result = m_threeDPixelShader->Initialize(device, hwnd, ThreeDPSFilename);
	if (!result)
		return false;

	m_threeDVertexShader = new VertexShaderClass();
	result = m_threeDVertexShader->Initialize(device, hwnd, ThreeDVSFilename, threeDPolygonLayout, threeDLayoutCount);
	if (!result)
		return false;

	m_twoDPixelShader = new PixelShaderClass();
	result = m_twoDPixelShader->Initialize(device, hwnd, TwoDPSFilename);
	if (!result)
		return false;

	m_twoDVertexShader = new VertexShaderClass();
	result = m_twoDVertexShader->Initialize(device, hwnd, TwoDVSFilename, twoDPolygonLayout, twoDLayoutCount);
	if (!result)
		return false;

	return true;
}

bool ShaderControllerClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix,
									ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor)
{
	bool result;

	result = m_vertexFocus->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
		return false;

	result = m_pixelFocus->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor);
	if (!result)
		return false;

	return result;
}

void ShaderControllerClass::Set2DShaders()
{
	m_vertexFocus = m_twoDVertexShader;
	m_pixelFocus = m_twoDPixelShader;
}

void ShaderControllerClass::Set3DShaders()
{
	m_vertexFocus = m_threeDVertexShader;
	m_pixelFocus = m_threeDPixelShader;
}