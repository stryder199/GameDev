////////////////////////////////////////////////////////////////////////////////
// Filename: VertexShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _VERTEXSHADERCLASS_H_
#define _VERTEXSHADERCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: VertexShaderClass
////////////////////////////////////////////////////////////////////////////////
class VertexShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

public: 
	VertexShaderClass();
	VertexShaderClass(const VertexShaderClass&);
	~VertexShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif