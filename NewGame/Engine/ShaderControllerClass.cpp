#include "ShaderControllerClass.h"
#include "PixelShaderClass.h"
#include "VertexShaderClass.h"
#include "WindowClass.h"
#include "D3DClass.h"

ShaderControllerClass::ShaderControllerClass()
{
}

ShaderControllerClass::~ShaderControllerClass()
{
}

bool ShaderControllerClass::Initialize()
{
	bool result;
	D3D11_INPUT_ELEMENT_DESC *posTexNorPolygonLayout, *posTexPolygonLayout, *posNorColPolygonLayout;
	int posTexNorLayoutCount, posTexLayoutCount, posNorColLayoutCount;

	WCHAR* MaterialPSFilename = L"shaders\\Material-PS.hlsl";
	WCHAR* MaterialVSFilename = L"shaders\\Material-VS.hlsl";

	WCHAR* TexturePSFilename = L"shaders\\Texture-PS.hlsl";
	WCHAR* TextureVSFilename = L"shaders\\Texture-VS.hlsl";

	WCHAR* TwoDPSFilename = L"shaders\\2D-PS.hlsl";
	WCHAR* TwoDVSFilename = L"shaders\\2D-VS.hlsl";

	posTexNorLayoutCount = 3;
	posTexNorPolygonLayout = new D3D11_INPUT_ELEMENT_DESC[posTexNorLayoutCount];
	//Now setup the layout of the data that goes into the shader
	//This setup needs to match the VertexType structure in the MeshClass and in the shader
	posTexNorPolygonLayout[0].SemanticName = "POSITION";
	posTexNorPolygonLayout[0].SemanticIndex = 0;
	posTexNorPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	posTexNorPolygonLayout[0].InputSlot = 0;
	posTexNorPolygonLayout[0].AlignedByteOffset = 0;
	posTexNorPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	posTexNorPolygonLayout[0].InstanceDataStepRate = 0;

	posTexNorPolygonLayout[1].SemanticName = "TEXCOORD";
	posTexNorPolygonLayout[1].SemanticIndex = 0;
	posTexNorPolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	posTexNorPolygonLayout[1].InputSlot = 0;
	posTexNorPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	posTexNorPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	posTexNorPolygonLayout[1].InstanceDataStepRate = 0;

	posTexNorPolygonLayout[2].SemanticName = "Normal";
	posTexNorPolygonLayout[2].SemanticIndex = 0;
	posTexNorPolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	posTexNorPolygonLayout[2].InputSlot = 0;
	posTexNorPolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	posTexNorPolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	posTexNorPolygonLayout[2].InstanceDataStepRate = 0;

	posNorColLayoutCount = 3;
	posNorColPolygonLayout = new D3D11_INPUT_ELEMENT_DESC[posNorColLayoutCount];
	//Now setup the layout of the data that goes into the shader
	//This setup needs to match the VertexType structure in the MeshClass and in the shader
	posNorColPolygonLayout[0].SemanticName = "POSITION";
	posNorColPolygonLayout[0].SemanticIndex = 0;
	posNorColPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	posNorColPolygonLayout[0].InputSlot = 0;
	posNorColPolygonLayout[0].AlignedByteOffset = 0;
	posNorColPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	posNorColPolygonLayout[0].InstanceDataStepRate = 0;

	posNorColPolygonLayout[1].SemanticName = "NORMAL";
	posNorColPolygonLayout[1].SemanticIndex = 0;
	posNorColPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	posNorColPolygonLayout[1].InputSlot = 0;
	posNorColPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	posNorColPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	posNorColPolygonLayout[1].InstanceDataStepRate = 0;

	posNorColPolygonLayout[2].SemanticName = "COLOR";
	posNorColPolygonLayout[2].SemanticIndex = 0;
	posNorColPolygonLayout[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	posNorColPolygonLayout[2].InputSlot = 0;
	posNorColPolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	posNorColPolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	posNorColPolygonLayout[2].InstanceDataStepRate = 0;

	posTexLayoutCount = 2;
	posTexPolygonLayout = new D3D11_INPUT_ELEMENT_DESC[posTexLayoutCount];
	posTexPolygonLayout[0].SemanticName = "POSITION";
	posTexPolygonLayout[0].SemanticIndex = 0;
	posTexPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	posTexPolygonLayout[0].InputSlot = 0;
	posTexPolygonLayout[0].AlignedByteOffset = 0;
	posTexPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	posTexPolygonLayout[0].InstanceDataStepRate = 0;

	posTexPolygonLayout[1].SemanticName = "TEXCOORD";
	posTexPolygonLayout[1].SemanticIndex = 0;
	posTexPolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	posTexPolygonLayout[1].InputSlot = 0;
	posTexPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	posTexPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	posTexPolygonLayout[1].InstanceDataStepRate = 0;

	result = m_threeDMaterialPS.Initialize(MaterialPSFilename, PixelShaderClass::ShaderType::THREEDMATERIAL);
	if (!result)
		return false;

	result = m_threeDMaterialVS.Initialize(MaterialVSFilename, posNorColPolygonLayout, posNorColLayoutCount, VertexShaderClass::ShaderType::THREEDMATERIAL);
	if (!result)
		return false;

	result = m_threeDTexturePS.Initialize(TexturePSFilename, PixelShaderClass::ShaderType::THREEDTEXTURE);
	if (!result)
		return false;

	result = m_threeDTextureVS.Initialize(TextureVSFilename, posTexNorPolygonLayout, posTexNorLayoutCount, VertexShaderClass::ShaderType::THREEDTEXTURE);
	if (!result)
		return false;

	result = m_twoDPS.Initialize(TwoDPSFilename, PixelShaderClass::ShaderType::TWOD);
	if (!result)
		return false;

	result = m_twoDVS.Initialize(TwoDVSFilename, posTexPolygonLayout, posTexLayoutCount, VertexShaderClass::ShaderType::TWOD);
	if (!result)
		return false;

	return true;
}

bool ShaderControllerClass::Render(int indexCount, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix,
									ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor)
{
	bool result;

	result = m_vertexFocus->Render(indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
		return false;

	result = m_pixelFocus->Render(indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor);
	if (!result)
		return false;

	D3DClass::getInstance()->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

	return result;
}

void ShaderControllerClass::Set2DShaders()
{
	m_vertexFocus = &m_twoDVS;
	m_pixelFocus = &m_twoDPS;
}

void ShaderControllerClass::Set3DTextureShaders()
{
	m_vertexFocus = &m_threeDTextureVS;
	m_pixelFocus = &m_threeDTexturePS;
}

void ShaderControllerClass::Set3DMaterialShaders()
{
	m_vertexFocus = &m_threeDMaterialVS;
	m_pixelFocus = &m_threeDMaterialPS;
}