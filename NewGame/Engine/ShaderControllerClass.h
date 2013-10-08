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

	bool Initialize(ID3D11Device* device, HWND hwnd, D3D11_INPUT_ELEMENT_DESC* threeDPolygonLayout, int threeDLayoutCount,D3D11_INPUT_ELEMENT_DESC* twoDPolygonLayout, 
						int twoDLayoutCount);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix,
					ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor);

	void Set3DShaders();
	void Set2DShaders();

private:
	VertexShaderClass* m_vertexFocus;
	PixelShaderClass* m_pixelFocus;

	VertexShaderClass m_twoDVertexShader;
	PixelShaderClass m_twoDPixelShader;

	VertexShaderClass m_threeDVertexShader;
	PixelShaderClass m_threeDPixelShader;
};