////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderControllerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERCONTROLLERCLASS_H_
#define _SHADERCONTROLLERCLASS_H_
#endif

/////////////////
// FORWARD DEC //
/////////////////
class VertexShaderClass;
class PixelShaderClass;

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
					ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor);

	void Set3DMaterialShaders();
	void Set3DTextureShaders();
	void Set2DShaders();

private:
	VertexShaderClass* m_vertexFocus;
	PixelShaderClass* m_pixelFocus;

	VertexShaderClass m_twoDVS;
	PixelShaderClass m_twoDPS;

	VertexShaderClass m_threeDMaterialVS;
	PixelShaderClass m_threeDMaterialPS;

	VertexShaderClass m_threeDTextureVS;
	PixelShaderClass m_threeDTexturePS;
};