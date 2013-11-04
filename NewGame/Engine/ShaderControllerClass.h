////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderControllerClass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

/////////////////
// FORWARD DEC //
/////////////////
class VertexShaderClass;
class PixelShaderClass;
class TextureClass;

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include "VertexShaderClass.h"
#include "PixelShaderClass.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderControllerClass
////////////////////////////////////////////////////////////////////////////////
class ShaderControllerClass{

public:
	ShaderControllerClass();
	~ShaderControllerClass();

	bool Initialize();
	bool Render(int indexCount, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix,
					TextureClass* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor);

	bool Render(int indexCount, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix,
		XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT4 color);

	bool Render(int indexCount, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix,
		TextureClass* texture, XMFLOAT4 color);

	bool Render(int indexCount, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix,
		TextureClass* texture);

	void Set3DMaterialShaders();
	void Set3DTextureShaders();
	void Set2DShaders();
	void SetTextShaders();

	VertexShaderClass* GetFocusVertex();
	PixelShaderClass* GetFocusPixel();

private:
	VertexShaderClass* m_vertexFocus;
	PixelShaderClass* m_pixelFocus;

	VertexShaderClass m_twoDVS;
	PixelShaderClass m_twoDPS;

	VertexShaderClass m_textVS;
	PixelShaderClass m_textPS;

	VertexShaderClass m_threeDMaterialVS;
	PixelShaderClass m_threeDMaterialPS;

	VertexShaderClass m_threeDTextureVS;
	PixelShaderClass m_threeDTexturePS;
};