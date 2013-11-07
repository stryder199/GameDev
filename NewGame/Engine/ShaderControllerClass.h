#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //
#include "VertexShaderClass.h"
#include "PixelShaderClass.h"

/////////////////
// FORWARD DEC //

using namespace DirectX;

class ShaderControllerClass{

public:
	ShaderControllerClass();
	~ShaderControllerClass();

	bool Initialize();
	bool Render(int indexCount, const XMFLOAT4X4& worldMatrix, TextureClass* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor);

	bool Render(int indexCount, const XMFLOAT4X4& worldMatrix, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT4 color);

	bool Render(int indexCount, const XMFLOAT4X4& worldMatrix, TextureClass* texture, XMFLOAT4 color);

	bool Render(int indexCount, const XMFLOAT4X4& worldMatrix, TextureClass* texture);

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