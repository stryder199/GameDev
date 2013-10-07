////////////////////////////////////////////////////////////////////////////////
// Filename: PixelShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PixelShaderClass_H_
#define _PixelShaderClass_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: PixelShaderClass
////////////////////////////////////////////////////////////////////////////////
class PixelShaderClass
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
	PixelShaderClass();
	PixelShaderClass(const PixelShaderClass&);
	~PixelShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd,WCHAR* psFilename);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, XMFLOAT4);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_lightBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif